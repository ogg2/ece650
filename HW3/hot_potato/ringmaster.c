#include "myfunc.h"
#include "potato.h"

int main(int argc, char *argv[]){
    if(argc!=4){
        fprintf(stderr,"should enter : ringmaster <port_num> <num_players> <num_hops>\n");
        return EXIT_FAILURE;
    }
    char* port = argv[1];
    int num_players = atoi(argv[2]);
    int num_hops = atoi(argv[3]);

    printf("Potato Ringmaster\n");
    printf("Players = %d\n",num_players);
    printf("Hops = %d\n",num_hops);

    int client_connection_fd[num_players];
    char client_port[num_players][256];
    char client_hostname[num_players][256];
    int socket_fd = setup_server(port);
    if(socket_fd<0){
        fprintf(stderr,"Error: fail to setup server\n");
        exit(EXIT_FAILURE);
    }

    //server_accept , get hostname
    for(int i=0;i<num_players;++i){
        client_connection_fd[i] = server_accpet(socket_fd,client_hostname[i]);
        if(client_connection_fd[i]<0){
            fprintf(stderr,"Error: fail to get client_connection_fd[%d]\n",i);
            exit(EXIT_FAILURE);
        }
    }


    /* 001: receive port from players */
    for(int i=0;i<num_players;++i){
        char buf[256];
        recv(client_connection_fd[i],buf,256,0);
        strcpy(client_port[i],buf);
        //printf("player %d 's port is %s \n",i, client_port[i]);
    }
    
    /* 002: send port back to players */
    for(int i=0;i<num_players;++i){
        /* send id */
        //har my_id[256];
        //sprintf(my_id,"%d",i);
        send(client_connection_fd[i],&i,sizeof(i),0);
 
        /* send neighbour's port & hostname & num_players*/
        int right_index = (i + num_players + 1) % num_players;
        send(client_connection_fd[i],client_port[right_index],256,0);
        send(client_connection_fd[i],client_hostname[right_index],256,0);
        send(client_connection_fd[i],&num_players,sizeof(num_players),0);
    }

    /* receive from player*/
    for(int i=0;i<num_players;++i){
        char buf[256];
        int len = recv(client_connection_fd[i],&buf,256,0);
        if(len<0){
            fprintf(stderr,"cannot hear from players\n");
            return EXIT_FAILURE;
        }
        printf("Player %d is ready to play\n",i);
    }

    /* send potato to random player*/
    srand((unsigned int) time(NULL));
    int random = rand() % num_players;
    
    Potato mypotato;
    mypotato.hops = num_hops;
    mypotato.length = 0;

    if(num_hops>0){
        printf("Ready to start the game, sending potato to player %d\n",random);
        send(client_connection_fd[random],&mypotato,sizeof(mypotato),0);
        


        /* receive potato */
        fd_set read_fds;
        int fdmax = -1;
        int len;

        FD_ZERO(&read_fds);
        
        // add to read_fds set
        for(int i=0;i<num_players;++i){
            FD_SET(client_connection_fd[i],&read_fds);
            fdmax = (client_connection_fd[i]>fdmax)?client_connection_fd[i]:fdmax;
        }

        if(select(fdmax+1,&read_fds,NULL,NULL,NULL) == -1){
            fprintf(stderr,"Error: select\n");
            return EXIT_FAILURE;
        }
        for(int i = 0;i <= fdmax; i++){
            if(FD_ISSET(i,&read_fds)){
                if(len = recv(i,&mypotato,sizeof(mypotato),0)>0){
                    break;
                }else if(len < 0){
                    fprintf(stderr,"Error: recv\n");
                    return EXIT_FAILURE;
                }
            }
        }
    }

    //test 
    //printf("mypotato.hops = %d\n",mypotato.hops);
    //printf("mypotato.length = %d\n",mypotato.length);

    printf("Trace of potato:\n");
    for(int i=0;i<mypotato.length;++i){
        printf("%d",mypotato.trace[i]);
        if(i!=mypotato.length-1)printf(",");
    }
    printf("\n");


    

    /* close */
    for(int i=0;i<num_players;++i){
        close(client_connection_fd[i]);
    }
    return 0;
}