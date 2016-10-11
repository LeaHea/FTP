#include "getArgs.h"
#include <mutex>

std::mutex m;
char *argsForExec[ MAX_ARGS ];
// std::vector<File*> *files = new std::vector <File*>;

void servArgs( int connfd, char *buf, std::vector<Client*> *cliIP ){

  if (std::string (buf) == "listserv")
    servList(cliIP, connfd);
  if (std::string (buf) == "cacheserv")
    servCache( cliIP);
  if (std::string (buf) == "shutdown")
    closeClient( connfd, cliIP );
  if (std::string (buf) == "shutserv")
    closeClients( cliIP );
  if (std::string (buf) == "shutdownserv")
    shutdown( cliIP );

}

void report( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  sprintf(buf, "client:\n");

  for (int i = 0; i < cliIP->size(); i++){
    sprintf(buf, cliIP->at(i)->toChar(), '\n');
    if( cliIP->at(i)->isCache() ){
      strcat(buf, "(cache client)\n" );
      strcat(buf, "clients cached:\n");
    }
    else
      strcat( buf, "(not a cache client)\n" );
    
    for (int j = 0; j < cliIP->at(i)->listOfClients()->size(); j++){
      strcat(buf, cliIP->at(i)->listOfClients()->at(j)->toChar());
      strcat(buf, "\nfiles shared:\n");}
    for (int k = 0; k < cliIP->at(i)->listOfFiles()->size(); k++){
      strcat(buf, cliIP->at(i)->listOfFiles()->at(k)->getName());
      strcat(buf, "\n");
    }  
  }

  if(write(connfd, buf, strlen(buf))<1)
    return;
  
}
  
void status( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  Client *client;

  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->getConnfd() == connfd){
      client = cliIP->at(i);
      break;
    }
  
  if(client->isCache()) 
    sprintf(buf, "you are a cache client.\n");
  else
    sprintf(buf, "you are a not a cache client.\n(no clients to show)\n");
  
  client->printFiles();
  client->printClients();
  
  write(connfd, buf, strlen(buf));
}

void download( char *read, int connfd ){
  write(connfd, read, strlen(read));
  doftp(connfd);  

}

void share( std::vector<Client*> *cliIP, int connfd, char* buf ){
  File *file = new File(buf);
  printf("at %s", file->getName());
  int place;
  for (int i = 0; i < cliIP->size(); i++)
    if( cliIP->at(i)->getConnfd() == connfd ){
      printf("at %d", i);
      cliIP->at(i)->addFile(file);
      place = i;
    } 
  
  cliIP->at(place)->printFiles();
}

void servCach( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->getConnfd() == connfd)
      if(cliIP->at(i)->isCache())  
	sprintf(buf, "You are a cache client.\n");
      else
	sprintf(buf, "You are a not a cache client.\n");
  
  write(connfd, buf, strlen(buf));
}

void servList( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  char mini [MAX_LINE] = {'\0'};
  bool active = 0;
  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->isActive()){
      active = 1;
      sprintf(mini, "IP: %s   Port: %d\n", cliIP->at(i)->getIP(),
	      cliIP->at(i)->getPortNum());
      strcat(buf, mini);
    }

  if(!active)
    sprintf(buf, "no clients signed on.\n");
  
  printf(buf);
  cliList( cliIP, connfd );
}

void cliList( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->getConnfd() == connfd)
      for (int k = 0; k < cliIP->at(i)->listOfFiles()->size(); k++){
	strcat(buf, cliIP->at(i)->listOfFiles()->at(k)->getName());
    	strcat(buf, "\n");
      }
  write(connfd, buf, strlen(buf)); 

}

void servCache( std::vector<Client*> *cliIP ){
  char ip [14];
  int portNum, connfd, numClients;
  bool gotOne = false;
  char buf [MAX_LINE] = {'\0'};
  int connPlace;

  printf("enter IP and port number of client to make cache:\n");
  printf("(xxx.xxx.xxx.xx xxxx)\n");
  std::cin >> ip;
  std::cin >> portNum;

  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->getIP() == std::string(ip))
      if(cliIP->at(i)->getPortNum() == portNum){
   	cliIP->at(i)->makeCache();
   	connfd = cliIP->at(i)->getConnfd();
	connPlace = i;
	gotOne = true;
      }
  if (!gotOne){
    m.lock();
    printf("please enter valid ip/port number.\n");
    servCache( cliIP );
  }
  m.unlock();
  sprintf(buf, "congrats, you're a cache client!\n");

  write(connfd, buf, strlen(buf));

  while( true ){
    printf("how many clients would you like to cache?\n");
    std::cin >> numClients;
    if (numClients < cliIP->size())
      break;
    printf("there are less than %d active clients...\n");
  }
  for (int j = 0; j < numClients; j++)
    while ( true ){
      printf("enter IP and port number for client to be cached#%d:\n", j+1);
      printf("(xxx.xxx.xxx.xx xxxx)\n");
      std::cin >> ip;
      std::cin >> portNum;
      for (int i = 0; i < cliIP->size(); i++)
	if(cliIP->at(i)->getIP() == std::string(ip))
	  if(cliIP->at(i)->getPortNum() == portNum){
	    cliIP->at(connPlace)->addClient(cliIP->at(i));
	    printf("made %s %d a cache client!\n", ip, portNum);
	    break;
	  }
	  else
	    printf("please enter valid ip/port number.\n");
    }
}
void shutdown( std::vector<Client*> *cliIP ){
  closeClients( cliIP );
  printf("goodbye!\n");
  exit (0);
  
}

void closeClients( std::vector<Client*> *cliIP ){
  printf("telling clients to shutdown...\n");
  char buf [MAX_LINE]= {'\0'};
  int connfd;

  for (int i = 0; i < cliIP->size(); i++){
    connfd = cliIP->at(i)->getConnfd();
    closeClient( connfd, cliIP );
    close(connfd);
  }
}

void list( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  char mini [MAX_LINE] = {'\0'} ;
  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->isActive()){
      sprintf(mini, "IP: %s  Port: %d\n", cliIP->at(i)->getIP(),
	      cliIP->at(i)->getPortNum());
      strcat(buf, mini);
    }  
  
  if(strlen(buf) == 0)
    sprintf(buf, "No clients signed on.\n");
  printf(buf);
  report(cliIP, connfd);
 
    
}
void sign( std::vector<Client*> *cliIP, int connfd, bool onOrOff ){
  char buf [MAX_LINE] = {'\0'};
  
  for (int i = 0; i < cliIP->size(); i++){
    if (cliIP->at(i)->getConnfd() == connfd)
      if ((cliIP->at(i)->isActive() && onOrOff) ||
	  (!cliIP->at(i)->isActive() && !onOrOff))
	sprintf(buf, "already ");
    cliIP->at(i)->sign(onOrOff);
  }
  strcat(buf, "signed ");
  if (onOrOff)
    strcat(buf, "on.\n");
  else
    strcat(buf, "off.\n");
  write(connfd, buf, strlen(buf));

}
void cach( std::vector<Client*> *cliIP, int connfd ){
  char buf [MAX_LINE] = {'\0'};
  char mini [MAX_LINE] = {'\0'};
  for (int i = 0; i < cliIP->size(); i++)
    if(cliIP->at(i)->isActive() && cliIP->at(i)->isCache()){
      sprintf(mini, "IP: %s   Port: %d\n", cliIP->at(i)->getIP(),
	      cliIP->at(i)->getPortNum());
      strcat(buf, mini);
    }  
  if(strlen(buf) == 0)
    sprintf(buf, "No cache clients signed on.\n");
  // write(connfd, buf, strlen(buf));
  servCach( cliIP, connfd );
  printf(buf);
}

void ls( int connfd ){
  char buf [MAX_LINE] = {'\0'};
  sprintf(buf, "ls");
  write(connfd, buf, strlen(buf));

}

void closeClient( int connfd, std::vector<Client*> *cliIP ){
  std::cout << "client shutting down..." << '\n';
  char buf [MAX_LINE]= {'\0'};
  for (int i = 0; i < cliIP->size(); i++)
    if (cliIP->at(i)->getConnfd() == connfd){
      cliIP->at(i)->quit();
      sprintf(buf, "goodbye!\n");
    }
  
  write(connfd, buf, strlen(buf)); 
  close( connfd );
}

bool isICommand( char *read ){
  
  if (std::string(read) == "list" ||
      std::string(read) == "cach" ||
      std::string(read) == "sign" ||
      std::string(read) == "download" ||
      std::string(read) == "ls" ||
      std::string(read) == "status" ||
      std::string(read) == "gbye"){
    return 1;
  }

  return 0;

}

char **argsForExecution( void ){
  int llen;
  int i = 0;

  char line[ MAX_LINE ];
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

  // start with the next character to find either a space-character                                                                                                                                
  // for the end of the string.                                                                                                                                                                    
  while( *end && ! isspace( *end ) )
    end++;

  *end = '\0';   // a c-string that begins with "start" and ends at "end".                                                                  

  // std::cout << argsForExec[i] << std::endl;                                                                                                                                                     
  return end + 1;

}

void doftp(int newsd)
{       
#define ACK                   2
#define NACK                  3
#define REQUESTFILE           100
#define COMMANDNOTSUPPORTED   150
#define COMMANDSUPPORTED      160
#define BADFILENAME           200
#define FILENAMEOK            400
#define MAXLINE 256
#define MAXSIZE 512     
  int i,fsize,fd,msg_ok,fail,fail1,req,c,ack;
  int no_read ,num_blks , num_blks1,num_last_blk,num_last_blk1,tmp;
  char fname[MAXLINE], fnameLen = 0;
  char out_buf[MAXSIZE];
  FILE *fp;
      
  no_read = 0;
  num_blks = 0;
  num_last_blk = 0; 

    
  /* Start servicing the client */ 
  
  /* get command code from client.*/
  /* only one supported command: 100 -  get a file */
  req = 0;
  if((readn(newsd,(char *)&req,sizeof(req))) < 0) {
    perror( "Read error" );
    exit(0);
  }
  req = ntohs(req);
  printf("server: client request code is: %d\n",req);
  if (req!=REQUESTFILE) {
    printf("server: unsupported operation. goodbye\n");
    /* reply to client: command not OK  (code: 150) */
    msg_ok = COMMANDNOTSUPPORTED; 
    msg_ok = htons(msg_ok);
    if((writen(newsd,(char *)&msg_ok,sizeof(msg_ok))) < 0) {
      perror( "Write error" );
      exit(0);
    }
    exit(0);
  }

  /* reply to client: command OK  (code: 160) */
  msg_ok = COMMANDSUPPORTED; 
  msg_ok = htons(msg_ok);
  if( writen(newsd,(char *)&msg_ok,sizeof(msg_ok))  < 0) {
    perror( "Write error" );
    exit(0);
  }
  
  fail = FILENAMEOK;
  if((fnameLen = read(newsd,fname,MAXLINE)) < 0) {
    perror("Filename read error" );
    fail = BADFILENAME ;
  }
  fname[ fnameLen ] = '\0';
   
  /* If server cant open file then inform client of this and terminate */
  if((fp = fopen(fname, "r")) == NULL) /*cant open file*/
    fail = BADFILENAME;

  tmp = htons(fail);
  if((writen(newsd,(char *)&tmp,sizeof(tmp))) < 0) {
    perror("Write error" );
    exit(0);   
  }
  if(fail == BADFILENAME) {
    printf("server cant open file\n");
    close(newsd);
    exit(0);
  }
  printf("server: filename is %s\n",fname);
  
  req = 0;
  if ( readn(newsd,(char *)&req,sizeof(req)) < 0) {
    perror( "Read error" );
    exit(0);
  }
  printf("server: start transfer command, %d, received\n", ntohs(req));

   
  /*Server gets filesize and calculates the number of blocks of 
      size = maxsize it will take to transfer the file. Also calculate
      number of bytes in the last partially filled block if any.  Send
      this info to client, receiving acks */

  printf("server: starting transfer\n");
  fsize = 0;ack = 0;   
  while ((c = getc(fp)) != EOF) {fsize++;}
  num_blks = fsize / MAXSIZE; 
  num_blks1 = htons(num_blks);
  num_last_blk = fsize % MAXSIZE; 
  num_last_blk1 = htons(num_last_blk);
  if((writen(newsd,(char *)&num_blks1,sizeof(num_blks1))) < 0) {
    perror( "Write error" );
    exit(0);
  }
  printf("server: told client there are %d blocks\n", num_blks);  
  if((readn(newsd,(char *)&ack,sizeof(ack))) < 0) {
    perror("ack read error " );
    exit(0); 
  }          
  if (ntohs(ack) != ACK) {
    printf("client: ACK not received on file size\n");
    exit(0);
  }
  if((writen(newsd,(char *)&num_last_blk1,sizeof(num_last_blk1))) < 0) {
    perror( "write error" );
    exit(0);
  }
  printf("server: told client %d bytes in last block\n", num_last_blk);  
  if( readn(newsd,(char *)&ack,sizeof(ack)) < 0) {
    perror( "ack read error" );
    exit(0); 
  }
  if (ntohs(ack) != ACK) {
    printf("server: ACK not received on file size\n");
    exit(0);
  }
  rewind(fp);    
  
  /* Actual file transfer starts  block by block*/       
       
  for(i= 0; i < num_blks; i ++) { 
    no_read = fread(out_buf,sizeof(char),MAXSIZE,fp);
    if (no_read == 0) {
      printf("server: file read error\n");
      exit(0);
    }
    if (no_read != MAXSIZE) {
      printf("server: file read error : no_read is less\n");
      exit(0);
    }
    if((writen(newsd,out_buf,MAXSIZE)) < 0) {
      perror("Write error: sending block" );
      exit(0);
    }
    if((readn(newsd,(char *)&ack,sizeof(ack))) < 0) {
      perror("ack read  error" );
      exit(0);
    }
    if (ntohs(ack) != ACK) {
      printf("server: ACK not received for block %d\n",i);
      exit(0);
    }
    printf(" %d...",i);
  }

  if (num_last_blk > 0) { 
    printf("%d\n",num_blks);
    no_read = fread(out_buf,sizeof(char),num_last_blk,fp); 
    if (no_read == 0) {
      printf("server: file read error\n");
      exit(0);
    }
    if (no_read != num_last_blk) {
      printf("server: file read error : no_read is less 2\n");
      exit(0);
    }
    if((writen(newsd,out_buf,num_last_blk)) < 0) {
      perror("File transfer error" );
      exit(0);
    }
    if((readn(newsd,(char *)&ack,sizeof(ack))) < 0) {
      perror("ack read  error" );
      exit(0);
    }
    if (ntohs(ack) != ACK) {
      printf("server: ACK not received last block\n");
      exit(0);
    }
  }
  else printf("\n");
                                                  
  /* FILE TRANSFER ENDS */
  printf("server: FILE TRANSFER COMPLETE on socket %d\n",newsd);
  fclose(fp);
  close(newsd);
}


/*
  To take care of the possibility of buffer limmits in the kernel for the
  socket being reached (which may cause read or write to return fewer characters
  than requested), we use the following two functions */  
   
int readn(int sd,char *ptr,int size)
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
