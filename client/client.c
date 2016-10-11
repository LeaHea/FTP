#include "client.h"
#include <mutex>

std::mutex m;

int main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  
  if (argc != 2) {
    fprintf( stderr, "Usage: %s <IPaddress>\n", argv[0] );
    exit( 1 );
  }
  
  sockfd = listen(argv);
  
  int num_threads = 10;
  std::thread t[num_threads];
  for ( ; ; ) {
    t[0] = std::thread (listenToServer, sockfd);
    for (int i = 1; i < num_threads; i++){
      t[i] = std::thread (talkToServer, sockfd);
      
    }
    for (int i=0; i < num_threads; i++)
      t[i].join();
  }
  
  return 0;
  
}
void listenToServer(int sockfd){
  for ( ; ; ){
    char recvline[MAXLINE] = {'\0'};
    if( read(sockfd, recvline, strlen(recvline)) < 0)
      break;
    
    handleArgs(recvline);
  }
}

void talkToServer(int sockfd){
  
  char **buf = {'\0'};
  buf = argsForExecution();
  
  for ( ; ; ){
    if( write(sockfd, *buf, MAXLINE) < 0 )
      break;
    char recvline[MAXLINE + 1] = {'\0'};
    
    read(sockfd, recvline, MAXLINE);    
    handleArgs(recvline);
    
    //fputs(recvline, stdout);
    
    buf = {'\0'};
    buf = argsForExecution();
    
  }
  
  close( sockfd );
  
  
}

int listen(char **argv){
  int sockfd;
  struct sockaddr_in myaddr, servaddr;
  
  // Create a socket end-point for communication.  We use the
  // IPv4 format and a stream socket. 
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf( stderr, "Socket error.  %s\n", strerror( errno ) );
    exit( 2 );
  }
  
  //	printf("client: binding my local socket\n");
  bzero(&myaddr, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(CLIENT_PORT_NUMBER);
  
  if (bind(sockfd ,(struct sockaddr *) &myaddr,sizeof(myaddr)) < 0) {
    printf("client: bind  error :%s\n",  strerror( errno )); 
    exit(0);
  }
  //	printf("client: starting connect\n");

  // build a profile for the server to whom we are going to
  // communicate.  
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  //	servaddr.sin_addr.s_addr = inet_addr(argv[2]);
  servaddr.sin_port   = htons(SERVER_PORT_NUMBER);
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    fprintf( stderr, "inet_pton error for %s\n", argv[1] );
    exit( 3 );
  }
  
  // Attempt to connect to the server.
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    fprintf( stderr, "connect error: %s\n", strerror( errno ) );
	    exit( 4 );
  }
	
  printf("Client connected to server.\n");
  
  return sockfd;

}
