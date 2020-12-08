#include <stdio.h>
#include <stdio.h>
#include <stdio.h>
#include <net/if.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>

#define BACKLOG 5
#define SERV_PORT 6666
#define SERV_IP_ADDR "192.168.247.131"
//#define SERV_IP_ADDR "192.168.253.130"
#define QUIT "quit"
#define BUFSIZES 128



int main(int argc ,const char * argv[]){
    //creat socket创建套接子
   int fd = -1;
     if((fd= socket(AF_INET,SOCK_STREAM,0)) < 0){
         perror("socket");
         exit(1);

     }
     fprintf(stderr,"create socket succeed\n");


    //创建地址信息结构体并填充
    struct sockaddr_in sin;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);
//    sin.sin_addr.s_addr = inet_pton(SERV_IP_ADDR);
    if(inet_pton(AF_INET,SERV_IP_ADDR,(void *)&sin.sin_addr.s_addr)!=1){
        perror("inet_pton");
        exit(1);
    }
    fprintf(stderr,"填充结构体成功\n");
    fprintf(stderr,"SERV_PORT= %d, SERV_IP_ADDR = %d\n",sin.sin_port,sin.sin_addr.s_addr);


	if(connect(fd,(struct sockaddr*)&sin, sizeof(sin)) < 0){
		perror("connect");
		exit(2);
		}

	fprintf(stderr,"客户端链接成功\n");
	//connect链接

	fd_set readfds;
	FD_ZERO(&readfds);



	char buf[BUFSIZES] ={};
	int ret = -1;
	int val = -1;

        FD_ZERO(&readfds);

        //create
        int maxfd = fd;
        struct timeval tout;

		while(1){
			FD_ZERO(&readfds);
			FD_SET(0,&readfds);
			FD_SET(fd,&readfds);

			tout.tv_sec = 5;
			tout.tv_usec =0;

			if(val = select(maxfd+1,&readfds,NULL,NULL,&tout) < 0){

				fprintf(stderr,"select error\n");
					perror("select");
					exit(1);
				}else if(val==0){
					fprintf(stderr,"timeout ...\n");
					continue;
				}else{
					if(FD_ISSET(0,&readfds)){
						bzero(buf,BUFSIZES);
						do{
							ret=read(0,buf,BUFSIZES);

							}while(ret <0 && EINTR==errno);
						if(val<0){
							perror("read");
							continue;
							}
						do{
							ret=send(fd,buf,strlen(buf),0);
							}while(ret<0&&EINTR==errno);
						if(ret <0){
							perror("send");
							continue;
							}else if(!ret){
								fprintf(stderr,"服务器正常关闭\n");
								break;
								}
							if(!strncasecmp(buf,QUIT,4)){
								fprintf(stderr,"我已经退出\n");

								break;
								}
							fprintf(stderr,"server:%s\n",buf);
						}
					}


		}





	close(fd);
    return 0;
}
