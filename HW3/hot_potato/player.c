#include "myfunc.h"
#include "potato.h"

int main(int argc, char*argv[]){
    if(argc!=3){
        fprintf(stderr,"should enter : player <machine_name> <port_num>\n");
        return EXIT_FAILURE;
    }
    int client_fd = client_connect_to_server(argv[1], argv[2]);
 
    /* assign a random server port*/
    /*
    int server_fd=setup_server_assign_port();
    int myport= what_is_port(server_fd);
    if(server_fd<0){
        fprintf(stderr,"player cannot set up server\n");
        return EXIT_FAILURE;
    }
    char myport_str[10];
    sprintf(myport_str, "%d", myport);
    */
    server_t my_server = setup_server_assign_port();
    int myport = my_server.port;
    int server_fd = my_server.server_fd;
    if(my_server.server_fd < 0 || my_server.port < 0){
        fprintf(stderr,"player cannot set up server, or cannot get a port\n");
        return EXIT_FAILURE;
    }
    char myport_str[256];
    sprintf(myport_str, "%d", myport);


    /* 001: send my port to ringmaster */
    int len = send(client_fd,myport_str,256,0);
    if(len < 0){
        fprintf(stderr,"Error: fail to send\n");
        return EXIT_FAILURE;
    }

    /* 002: receive my id & neighbours' port and hostname */
    int player_id;
    recv(client_fd,&player_id,sizeof(player_id),0);
    //printf("myID is %s, my port is %s\n",my_id,myport_str);

    /* 002: continue */
    char right_port[256];
    char right_hostname[256];
    int num_players;
    recv(client_fd,&right_port,256,0);
    recv(client_fd,&right_hostname,256,0);
    recv(client_fd,&num_players,256,0);
    //printf("myID is %s, my right hostname is %s, my right port is %s,total num = %d\n",my_id,right_hostname,right_port,num_players);


    /* connect to my right neighbour */
    int right_connection_fd = client_connect_to_server(right_hostname,right_port);

    /* accpet my left neighbour */
    char left_hostname[256];
    int left_connection_fd = server_accpet(server_fd,left_hostname);
    //printf("left hostname is %s\n",left_hostname);


    printf("Connected as player %d out of %d total players\n",player_id,num_players);
    /* test: send sth*/
    send(right_connection_fd,&player_id,256,0);

    /* recv it */
    char buffer[256];
    recv(left_connection_fd,&buffer,256,0);
    //printf("mt ID is %s , I get a message from ID %s\n",player_id,buffer);

    /* send my ID to ringmaster*/
    send(client_fd,&player_id,256,0);


    srand((unsigned int)time(NULL)+player_id);


    Potato mypotato;
    mypotato.hops = -1;
    mypotato.length = -1;

    while(1){
        /* receive potato */
        fd_set read_fds;
        int fdmax = -1;
        int len = 0;

        FD_ZERO(&read_fds);

        // add to read_fds set
        FD_SET(left_connection_fd,&read_fds);
        FD_SET(right_connection_fd,&read_fds);
        FD_SET(client_fd,&read_fds);
        if(left_connection_fd > right_connection_fd) fdmax = left_connection_fd;
        if(client_fd > fdmax) fdmax = client_fd;

        if(select(fdmax+1,&read_fds,NULL,NULL,NULL) == -1){
            fprintf(stderr,"select fail\n");
            return EXIT_FAILURE;
        }
        for(int i = 0;i <= fdmax; i++){
            if(FD_ISSET(i,&read_fds)){
                //printf("%d : catch the potato\n",player_id);
                if(len = recv(i,&mypotato,sizeof(mypotato),0)>0){
                    break;
                }else if(len < 0){
                    fprintf(stderr,"Error: recv\n");
                    return EXIT_FAILURE;
                }
            }
        }

        if(mypotato.hops==0||len == 0){
            break;
        }

        mypotato.trace[mypotato.length]=player_id;
        mypotato.hops -= 1;
        mypotato.length += 1;

        /* send potato */
        if(mypotato.hops==0){
            printf("I'm it\n");
            send(client_fd,&mypotato,sizeof(mypotato),0);
            break;
        }else{
            int next_player = rand() % 2;
            int next_player_fd = next_player?left_connection_fd:right_connection_fd;
            int next_player_id = next_player?(player_id+num_players-1)%num_players:(player_id+1)%num_players;
            printf("Sending potato to %d\n",next_player_id);
            send(next_player_fd,&mypotato,sizeof(mypotato),0);
            
        }
    }
    
    
    /*
    //test : receive potato fron ringmaster
    Potato mypotato;
    mypotato.hops = -1;
    mypotato.length = -1;
    len = recv(client_fd,&mypotato,sizeof(mypotato),0);
    if(len<0){
        fprintf(stderr,"cannot receive \n");
        return(EXIT_FAILURE);
    }
    printf("hops = %d\n",mypotato.hops);

    mypotato.trace[mypotato.length]=player_id;
    mypotato.hops -= 1;
    mypotato.length += 1;

    printf("hops = %d\n",mypotato.hops);
    
    //test :send back to ringmaster
    send(client_fd,&mypotato,sizeof(mypotato),0);
    */



    /*
    Potato mypotato;
    mypotato.hops = -1;
    mypotato.curr_hop = -1;
    len = recv(client_fd,&mypotato,256,0);
    if(len<0){
        fprintf(stderr,"cannot receive \n");
        return(EXIT_FAILURE);
    }
    if(len == 0){
        printf("%d: I can't hear\n",player_id);
    }
    if(mypotato.hops!=-1){
        printf("%d : potato's hop is %d\n",player_id,mypotato.hops);
        //strcpy(mypotato.trace[mypotato.curr_hop],player_id);
        //mypotato.curr_hop++;
        //printf("trace[0] %s\n",mypotato.trace[0]);
    }
    */
    close(client_fd);
    close(left_connection_fd);
    close(right_connection_fd);
    return 0;
}