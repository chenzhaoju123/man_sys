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
#define QUIT "quit"
#define BUFSZIES 128



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
	if(inet_pton(AF_INET,"0.0.0.0",(void *)&sin.sin_addr.s_addr)!=1){
		perror("inet_pton");
		exit(1);
		}
	fprintf(stderr,"填充结构体成功\n");

	if(connect(fd,(struct sockaddr*)&sin, sizeof(sin)) < 0){
		perror("connect");
		exit(2);
		}
	
	fprintf(stderr,"客户端链接成功\n");
	//connect链接

	fd_set readfds;
	FD_ZERO(&readfds);
	
	
//数据传输
	char buf[BUFSIZS] ={};
	int ret = -1;
	int val = -1;
	epoll_create(1);


	
	while(1){
		if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientlen)) < 0){
				perror("accept");
				exit(4);
			}
		}
	while(1){
		do{
			ret = recv(acceptfd, buf, sizeof(buf), 0);
			}while(ret <0 && EINTR = =errno);
		if (ret <0){
			perror("recv");
			exit(5);
			}else if(!ret){
				fprintf("对方正常关闭\n");
				break;
			}
		fprintf(stderr,"buf = %s\n",buf);
//断开连接
		if(!strncmp(buf, QUIT, strlen(QUIT))){
			fprintf(stderr,"客户端%s已经退出\n",(char *)inet_ntoa(clientaddr.sin_addr));
			close(acceptfd);
			break;
			}
		}
		


	close(sockfd);
    return 0;
}
