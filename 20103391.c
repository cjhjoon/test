/*
 * echo_select_server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>

#define BUFSIZE 100

int tcpConnect(char *, int);
void error_handling(char *message);

int main(int argc, char **argv)
{
	int serv_sock;
	struct sockaddr_in serv_addr;
	char message[BUFSIZE];
	char ID[100];
	int str_len;
	
	if(argc!=3)
	{
		printf("Usage : %s <port> <name>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//if (bind(tcpServ_sock, (struct sockaddr *)&tcpServer_addr, sizeof tcpServer_addr) < 0) {
	//	perror("bind");
	//	exit(1);
	//}

	if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
  
	// 프로필 출력
	printf( "Student ID : 20103391\n" );
	printf( "Name : Choi JunHwan\n" );
	
	// ID 복사
	strcpy( ID, argv[2] );

	char host[100];
	int port;
	char portnum;
	
	fd_set reads, temps;
	int fd_max;

	FD_ZERO(&reads);
	FD_SET(fileno(stdin), &reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;
	portnum = argv[1];

	while(1)
	{
		int fd;
		int clnt_sock, clnt_len;
		struct sockaddr_in clnt_addr;

		temps = reads;
	
		select(fd_max+1, &temps, 0, 0, NULL);
		
		memset( message, '\0', sizeof(message) );

		for (fd=0; fd<fd_max+1; fd++)
		{
			if ( FD_ISSET( fd, &temps ) )
			{
				if ( fd == serv_sock ) // 연결 요청인 경우
				{
					clnt_len = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_len);
					FD_SET(clnt_sock, &reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("connection from host 127.0.0.1, port %d, socket %d \n", port,clnt_sock);
					
						//sprintf( Info,"connection from host 127.0.0.1, port %d, socket %d \n", port, clnt_sock);

				}
				else if ( fd == fileno(stdin) )
				{
					fgets( message, sizeof (message), stdin );
					
					if ( strncmp(message, "@talk", 5) == 0 )
					{
						char Info[1024];
						sscanf( &message[6], "%s %d", host, &port);

						if(clnt_sock > 0)
						{
							printf( "Close Socket %d\n", clnt_sock );
							close(clnt_sock);
							FD_CLR(clnt_sock, &reads);
						}
							
						clnt_sock = tcpConnect(host, port);
						FD_SET(clnt_sock, &reads);

						if( fd_max < clnt_sock )
							fd_max = clnt_sock;
						
						
					}
					else if ( strncmp(message, "@quit", 5) == 0 )
					{	
						printf( "Connection Closed %d\n", clnt_sock );
						close(clnt_sock);
						clnt_sock = -1;
						fd_max = serv_sock;
						continue;
					}
					else
					{
						//그밖에 메세지들은 그냥 전송하기
						char sendmessage[100];

						sprintf( sendmessage, "%s >> %s", ID, message );
						if( send( clnt_sock, sendmessage, strlen(sendmessage), 0 ) < 0)
						{
							printf("Sending is failed.\n");
							close(clnt_sock);
						}
					}
				}
				else
				{
					str_len = read(fd, message, BUFSIZE);
					if(str_len ==0) // 연결 종료 요청인 경우
					{ 
						FD_CLR(fd, &reads);
						close(fd);
						printf("Close Socket %d\n", fd);
					}
					printf( "%s\n", message );
				}
			}
		}
	}
}

int tcpConnect(char *hostName, int port)
{
	struct sockaddr_in server_addr;
	struct hostent* host;
	int sock;

	if ( ( sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0)
		error_handling( "connecting socket error" );

	host = gethostbyname( hostName );
	if( host == 0 )
		error_handling( "unknown host\n" );

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	memcpy((void *) &server_addr.sin_addr, host->h_addr, host->h_length);
	server_addr.sin_port=htons(port);

	if( connect( sock, (struct sockaddr *)&server_addr, sizeof(server_addr) ) < 0 )
		error_handling( "TCP connect() error" );

	printf("connection is made successfully\n");

	return sock;
}

void error_handling(char *message)
{
	printf( "%s\n", message );
	exit(1);
}
