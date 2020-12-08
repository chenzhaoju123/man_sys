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
 #include <pthread.h>


#define BACKLOG    5
#define SERV_PORT  6666
#define SERV_IP_ADDR "192.168.1.123"
#define QUIT "quit"
#define BUFSZIES 128

struct my_t{
	int newfd;
	struct sockaddr_in cin;
};

struct my_t  info;


void cli_info(struct sockaddr_in cin){

}



void cli_data_handle(void * arg);




int main(int argc ,const char * argv[]){


	int fd = -1 ;
	if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
		}
	fprintf(stderr,"创建套接字成功\n");

	//设置允许地址快速重新启动
	int b_reuse = 1;
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(int)) !=0){
		perror("setsockopt");
		exit(2);
		}
	fprintf(stderr ,"开启地址快速重用成功");

	//初始化结构体
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(SERV_PORT);
	sin.sin_addr.s_addr = INADDR_ANY;
	fprintf(stderr,"初始化结构体成功\n");

	//绑定ip
	if(bind(fd,(struct sockaddr *)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(3);
		}

	fprintf(stderr,"绑定IP成功\n");

	//监听
	if(listen(fd,BACKLOG+10) < 0 ){
		perror("listen");
		exit(4);
		}
	fprintf(stderr,"监听成功\n");

	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	int newfd = -1;
	pthread_t tid = -1;

	while(1){

		if((newfd = accept(fd, (struct sockaddr *) &cin, &addrlen)) < 0 ){
			perror("accept");
			break;
			}
	}

	fprintf(stderr,"连接%d成功",newfd);

	//填充结构体
	info.newfd = newfd;
	memcpy(&info.cin,&cin,sizeof(cin));

	//已连接客户端信息
//	cli_info(cin);

	//创建线程用于通信
	pthread_create(&tid,NULL,(void *)cli_data_handle,(void *)&info);


	return 0;

}



void cli_data_handle(void * arg){

	int ret = -1;
	char buf[BUFSZIES] = {};
	pthread_detach(pthread_self());
	struct my_t pinfo = *((struct my_t *)arg);
	while(1){
		bzero(&buf,sizeof(buf));
		do{
			ret = recv(pinfo.newfd, buf, sizeof(buf), 0);
			}while(ret < 0 && EINTR == errno);

		if(ret <0 ){
			perror("recv");
			break;
			}else if(!ret){
				fprintf(stderr,"对方正常关闭\n");
				break;
				}
		if(!strncasecmp(buf,QUIT,strlen(QUIT))){
			fprintf(stderr,"客户端(%s:%d)已经退出\n",inet_ntoa(pinfo.cin.sin_addr),ntohs(pinfo.cin.sin_port));
			break;
			}
		fprintf(stderr,"客户端(%s:%d):(%s)\n",inet_ntoa(pinfo.cin.sin_addr),ntohs(pinfo.cin.sin_port),buf);
		}

	close(pinfo.newfd);
}




























