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
#include <stdio.h>
#include <sqlite3.h>

#define ADM 1
#define USER 2

#define BACKLOG 5
#define BUFSIZES 128
#define NAMELEN  3
#define ADDRLEN  20
#define DEPARTLEN 10
#define COUNT 0

#define hand_err(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)

void OPEN_SQ(void);


int main(int argc , const char* argv[]){
	int sockfd = -1;
//	int ret = -1;
	int i = -1;
	int acceptfd = -1;
	//MSG msg;
	//MSG_L msg_l;

	while(1){
		if(argc < 3){
			printf("重新输入，缺少ip，端口号\n");
			exit(-1);
			}
        break;
		}

	OPEN_SQ();
	//创建套接字

	if((sockfd=socket(AF_INET,SOCK_STREAM,0 ))<0){
			hand_err("socket");
		}
	printf("建立套接字成功");

	int b_reuse = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(int))!= 0){
	    hand_err("setsockopt");
    }
	//创建结构体，填充结构体
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port   = htons(atoi(argv[2]));
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	struct sockaddr_in cin;
	socklen_t addrlen = sizeof(cin);
	printf("填充结构成功\n");
	//建立绑定成功
	if(bind(sockfd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		hand_err("bind");
		}
	printf("绑定成功\n");
	//监听
	if(listen(sockfd,BACKLOG)<0){
		hand_err("listen");
		}
	printf("监听成功\n");



//创建epoll
    int ret_event = -1;//return epoll_wait()
    int count = 0,temp = -1;
	struct epoll_event ev, ev_r,events[1000];



	int epfd = epoll_create(1000);
	if(epfd == -1){
		hand_err("epoll_create");
		}
	ev.events = EPOLLIN|EPOLLET;
	ev.data.fd = sockfd;

    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
    if(ret < 0 ){
        hand_err("epoll_ctl");
    }

	while(1){
		ret_event = epoll_wait(epfd,events,1000,-1);
        if(-1 == ret_event){
            hand_err("epoll_wait");
        }
        if(0 == ret_event){
            printf("timeout...\n");
            continue;
        }
        //printf("i=%d",i++);
		for(i=0;i < ret_event;i++){
			int fd =events[i].data.fd;
			if(fd == sockfd&&events[i].events&EPOLLIN )
            {
				if((acceptfd = accept(sockfd,(struct sockaddr*)&cin,&addrlen)) < 0){
					hand_err("connect");
					}
//                printf(" success\n");
                count++;
//                printf("count= %d\n",count);
				ev.events = EPOLLIN|EPOLLET;
				ev.data.fd = acceptfd;
				epoll_ctl(epfd,EPOLL_CTL_ADD,acceptfd,&ev);
                printf("accept success\n");

			}
            else if(events[i].events&EPOLLIN){
				temp = events[i].data.fd;
                if(temp < 0){
                    hand_err("temp");
                }
                char recvbuf[1024];
                memset(recvbuf,0,sizeof(recvbuf));

                int ret = read(temp, recvbuf, sizeof(recvbuf));
                if(-1 == ret){
                    hand_err("read have some problem");

                printf("read success\n");
                }
				else if(0 == ret)
                {
                    ev_r = events[i];
					epoll_ctl(epfd,EPOLL_CTL_DEL,temp,&ev_r);
					close(temp);
					printf("client close...\n");
                }
				if(  ret > 0  )
                {
					printf("client: %s\n",recvbuf);
					ev.events = EPOLLIN|EPOLLET;
					ev.data.fd = temp;
					epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
					}
				}
            }


		//	else					{}



    }
    close(sockfd);
	return 0 ;
}


void OPEN_SQ(void)
{
	sqlite3 *db;
	char* errmsg;
	if(sqlite3_open("my.db",&db)!=SQLITE_OK){
		printf("%s \n",sqlite3_errmsg(db));
		exit(10);
		}
	else
		printf("open my.db succed\n");

	if(sqlite3_exec(db,"create table if not exists user (name text primary key, pass text);",NULL,NULL,&errmsg)!=SQLITE_OK){
		printf("%s\n",errmsg);
		}
	else

			printf("create user table success\n");


	if(sqlite3_exec(db,"create table if not exists adm (name text primary key, pass text);",NULL,NULL,&errmsg)!=SQLITE_OK){
		printf("%s\n",errmsg);
		}
	else

			printf("create adm table success\n");


	if(sqlite3_exec(db,"create table if not exists record (name text primary key, pass text);",NULL,NULL,&errmsg)!=SQLITE_OK){
		printf("%s\n",errmsg);
		}
	else

			printf("create record table success\n");


	if(sqlite3_exec(db,"create table if not exists info (name char primary key, addr char,age short,tel_num int ,salary int , aepart char , level int,num int , id_num int);",NULL,NULL,&errmsg)!=SQLITE_OK){
		printf("%s\n",errmsg);
		}
	else
			printf("create adm table success\n");



}











