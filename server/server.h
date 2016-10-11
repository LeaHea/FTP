#ifndef _SERVER
#define _SERVER

#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<ctype.h>
#include<thread>
#include"getArgs.h"
#include"client.h"

#define PORT_NUMBER 8160
#define MAXLINE 200
#define MAXCLIENTS 5

#define LISTENQ 5                                     

void handleArgs ( int listenfd, std::vector <Client*> *cliIP );
void listenToClient( int listenfd, std::vector <Client*> *cliIP );
void doSomething(int connfd, std::vector <Client*> *cliIP );
int listen( void );


#endif
