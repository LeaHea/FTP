#ifndef _CLIENT
#define _CLIENT

#include<sys/types.h>
#include<sys/socket.h>  /* basic socket definitions */
#include<netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include<arpa/inet.h>

#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<iostream>
#include<thread>
#include"getArgs.h"

#define SERVER_PORT_NUMBER 8160
#define CLIENT_PORT_NUMBER 8162
#define MAXLINE 200

// If the server is running on the local machine, you can do:                                                                                                                               

// ./client.x 127.0.0.1                                                                                                                                                                     
void talkToServer(int sockfd);
void listenToServer(int sockfd);
int listen(char **argv);

#endif
