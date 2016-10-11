#ifndef _GETARGS
#define _GETARGS

#include<sys/wait.h>

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <errno.h>

const int MAX_LINE = 1024;
const int MAX_ARGS = 20;

char *getArg(char line[], char *argsForExec [], int llen, int i);

char **argsForExecution( void );
void download( char *server);
int readn(int sd, char *ptr, int size);
int writen(int sd,char *ptr,int size);

void ls();
void lsdir(char *dir);
void handleArgs(char *fromServer);

#endif
