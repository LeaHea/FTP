#ifndef _GETARGS
#define _GETARGS

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <vector>
#include "client.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

const int MAX_LINE = 1024;
const int MAX_ARGS = 20;
//char *argsForExec[ MAX_ARGS ];

char *getArg(char line[], char *argsForExec [], int llen, int i);
char **argsForExecution( void );
bool isICommand( char *read );
void download( char *read, int connfd );
void servArgs( int connfd, char *buf, std::vector<Client*> *cliIP );
void servList( std::vector<Client*> *cliIP, int connfd );
void servCache( std::vector<Client*> *cliIP );
void servCach( std::vector<Client*> *cliIP, int connfd );
void report( std::vector<Client*> *cliIP, int connfd );
void status( std::vector<Client*> *cliIP, int connfd );
void cliList( std::vector<Client*> *cliIP, int connfd );
void share( std::vector<Client*> *cliIP, int connfd, char* buf );
void shutdown( std::vector<Client*> *cliIP );
void closeClient( int connfd, std::vector<Client*> *cliIP );
void closeClients( std::vector<Client*> *cliIP );
void list( std::vector<Client*> *cliIP, int connfd );
void ls( int connfd );
void cach( std::vector<Client*> *cliIP, int connfd );
void sign( std::vector<Client*> *cliIP, int connfd, bool onOrOff );
void doftp(int connfd);
int readn(int sd,char *ptr,int size);
int writen(int sd,char *ptr,int size);

#endif
