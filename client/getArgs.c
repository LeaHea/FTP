#include"getArgs.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

//const int MAX_LINE = 1024;
char *argsForExec[ MAX_ARGS ];  



void handleArgs(char *fromServer){
 
  if (std::string(fromServer) == "goodbye!\n"){
    printf("goodbye!\n");
    exit(0);
  }
  if (fromServer[0] == 'l' && fromServer[1] == 's')
    if (strlen(fromServer) == 2)
      ls();
    else
      lsdir(fromServer + 2);
  if (fromServer[0] == 'd')
    download(fromServer);
}
  
/* Due to the fact that buffer limits in kernel for the socket may be 
   reached, it is possible that read and write may return a positive value
   less than the number requested. Hence we call the two procedures
   below to take care of such exigencies */

int readn(int sd, char *ptr, int size)
{
  int no_left,no_read;
  no_left = size;
  while (no_left > 0) { 
    no_read = read(sd,ptr,no_left);
    if(no_read <0)  return(no_read);
    if (no_read == 0) break;
    no_left -= no_read;
    ptr += no_read;
  }
  return(size - no_left);
}


int writen(int sd,char *ptr,int size)
{         int no_left,no_written;
  no_left = size;
  while (no_left > 0) { 
    no_written = write(sd,ptr,no_left);
    if(no_written <=0)  return(no_written);
    no_left -= no_written;
    ptr += no_written;
  }
  return(size - no_left);
  
}
void lsdir(char *dir){
  
  int pfd[2], pid;
 
  if ( pipe (pfd) == -1 )
    printf( "Could not create a pipe" );
  switch ( pid = fork() ) {
  case -1: 
    printf( "Fork failed." );
    break;
  case  0: 
    if ( close( 0 ) == -1 )
      printf( "Could not close stdin I" );
    dup(pfd[0]);
    if ( close (pfd[0]) == -1 || close (pfd[1]) == -1 )
      printf( "Could not close pfds I" );
    execlp("ls", "ls", (char* const* ) dir, NULL);
    printf( "unsuccessfull execlp I!!");
    
  }
}


void ls(){
  
  int pfd[2], pid;

  if ( pipe (pfd) == -1 )
    printf( "Could not create a pipe" );
  switch ( pid = fork() ) {
  case -1: 
    printf( "Fork failed." );
    break;
  case  0: 
    if ( close( 0 ) == -1 )
      printf( "Could not close stdin I" );
    dup(pfd[0]);
    if ( close (pfd[0]) == -1 || close (pfd[1]) == -1 )
      printf( "Could not close pfds I" );
    execlp("ls", "ls", NULL);
    printf( "unsuccessfull execlp I!!");
    
    }
}

char **argsForExecution( void ){
  int llen;
  int i = 0;

  char line[ MAX_LINE ] = {'\0'};
  char *start = line;

  std::cin.getline( line, MAX_LINE, '\n' );
  llen = strlen( line );

  while ( *start != '\0' && *getArg( start, argsForExec, llen, i)){
    start = getArg( start, argsForExec, llen, i);
    argsForExec[i] = start;
    i++;
  }

  return argsForExec;

}

char *getArg(char line[], char *argsForExec [], int llen, int i){
  // skip leading spaces.
  char *start;
  for( start = line; *start && isspace( *start ); ++start )
    ;

  char *end = start;
  if (i == 0){
    return start;
  }
  while( *end && ! isspace( *end ) )
    end++;

  *end = '\0';   // a c-string that begins with "start" and ends at "end".        
  return end + 1;

}
void download(char *fromServer){

}            
