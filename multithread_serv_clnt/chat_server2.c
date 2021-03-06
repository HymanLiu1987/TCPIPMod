/////////////////////
/////////////////////
//File:chat_server2.c
//version:1.0
//Author:Hyman
//Date:2016/11/15
//Description:这是一个共享buff的多线程的服务端框架，这种设计本身不够合理，每个客户端在和服务端交互完之后服务器才会
//和下一个客户端进行会话，本例子仅供试验说明问题使用。
/////////////////////
/////////////////////
/////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#define BUF_SIZE 30

char buff[BUF_SIZE];
pthread_mutex_t mt;

void* clnt_handling(void* param)
{
   int str_len;
   int clnt_sock=*((int*)param);
   pthread_mutex_lock(&mt); 
   while(1)
   { 
        str_len=read(clnt_sock,buff,BUF_SIZE);
        if(str_len<=0)
            break;
        write(clnt_sock,buff,str_len);
   }
   pthread_mutex_unlock(&mt); 
}

int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_addr_sz;
    pthread_t tid;
    pthread_mutex_init(&mt,NULL);

    if(argc!=2)
    {
        printf("Usage %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(AF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
    
    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    listen(serv_sock,5);
    while(1)
    {
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
        pthread_create(&tid,NULL,clnt_handling,(void*)&clnt_sock);
        pthread_detach(tid);
    }
    
    pthread_mutex_destroy(&mt);
    close(serv_sock);
    
    return 0;
}
