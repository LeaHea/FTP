#include "server.h"

std::vector <Client*> *cliIP = new std::vector<Client*>;
bool lock = false;

int main(int argc, char **argv)
{
  int listenfd, connfd;
  socklen_t len;
  struct sockaddr_in  servaddr;
  int pid;
  char **buf = {'\0'};

  // creates vector of ips of clients
  listenfd = listen();
  int num_threads = 10;

  std::thread t[num_threads];

  for ( ; ; ) {
    t[0] = std::thread ( handleArgs, listenfd, cliIP );
    for (int i = 1; i < num_threads; i++)
      t[i] = std::thread ( listenToClient, listenfd, cliIP );
    
    for (int i=0; i < num_threads; i++)
      t[i].join();
  
  }

  exit(0);
  
}
void handleArgs ( int listenfd, std::vector <Client*> *cliIP ){
   for ( ; ; ){
    char *buf = {'\0'};
    buf = *argsForExecution();
    strcat(buf, "serv");
    servArgs(listenfd, buf, cliIP );
  }
  
}

void listenToClient( int listenfd, std::vector <Client*> *cliIP ){

  int connfd;
  socklen_t len;
  struct sockaddr_in  cliaddr;

  char buff[MAXLINE] = {'\0'};

  len = sizeof(cliaddr);
  // establish a connection with an incoming client.
  if( ( connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len) ) < 0 ) {
    fprintf( stderr, "\nAccept failed.  %s\n", strerror( errno ) );
    exit( 1 );
  }

  const char *ip = inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff));
  int pn = ntohs(cliaddr.sin_port);

  Client *client = new Client(ip, pn, connfd);

  cliIP->push_back(client);
 
  printf("\nconnection from %s, port %d\n",
	 inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
	 ntohs(cliaddr.sin_port));
  
  doSomething(connfd, cliIP);

  printf("\n***connection closed from %s, port %d***\n",
	 inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
	 ntohs(cliaddr.sin_port));

}
void doSomething(int connfd, std::vector <Client*> *cliIP ){

  int i;
  char buf [MAXLINE]= {'\0'}; 
  char temp [MAXLINE]= {'\0'}; 

  for ( ; ; ){
    i = 0;    
    char out_buf[MAXLINE] = {'\0'};
    if( read(connfd, buf, MAXLINE) < 0)
      break;    
    
    if(std::string(buf) == "list")
      list( cliIP, connfd );
    
    else if(buf[0] == 'l' && buf[1] == 's')
      if(buf + 2 == '\0')
	ls( connfd );
      else{
	strcpy(temp, buf);
	strcat(temp, buf + 3);
	std::cout << temp << '\n';
	write(connfd, temp, strlen(temp));
  
      }
    else if(std::string(buf) == "sign")
      sign( cliIP, connfd, 1 );
    
    else if(std::string(buf) == "gbye")
      sign( cliIP, connfd, 0 );

    else if(std::string(buf) == "list")
      report( cliIP, connfd );
    
    else if(std::string(buf) == "share")
      share( cliIP, connfd, buf + 6);
    
    else if(std::string(buf) == "cach")
      cach( cliIP, connfd );
    
    else if(std::string(buf) == "status")
      status( cliIP, connfd );
    
    else if((buf[0]) == 'd')
      if((buf[1]) == 'o')
	if((buf[7]) == 'd')
	  download( buf, connfd );
        
    else
      sprintf(out_buf, "please enter valid command\n", out_buf);   
    
    if ( write(connfd, out_buf, strlen(out_buf)) < 0)
	break;
    
    char buf [MAXLINE] = {'\0'};  

  }
     
  close(connfd);  
    
}

int listen( void ){
  struct sockaddr_in  servaddr;
  int listenfd;
  std::cout << "\nListening on port: " << PORT_NUMBER << '\n';
  // Create an end-point for IPv4 Internet Protocol                                                                           
  if( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
    fprintf( stderr, "socket failed.  %s\n", strerror( errno ) );
    exit( 1 );
  }
  int optval = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
  
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;             // Communicate using the Internet domain (AF_INET)                               
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept connections from?                                   
  servaddr.sin_port = htons(PORT_NUMBER);  // Which port should the server listen on?                                         
  
  // Bind the server end-point using the specifications stored in "serveraddr"                                                
  if( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    fprintf( stderr, "Bind failed.  %s\n", strerror( errno ) );
    exit( 1 );
 }
  
  // Listen on the in-comming connections; pile up at most LISTENQ number of connections.                                     
  if( listen(listenfd, LISTENQ) < 0 ) {
    fprintf( stderr, "Listen failed.  %s\n", strerror( errno ) );
    exit( 1 );
  }
  return listenfd;
}
