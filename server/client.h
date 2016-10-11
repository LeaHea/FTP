#ifndef _CLIENT
#define _CLIENT
#include<cstring>
#include<cstdio>
#include<vector>
#include"file.h"

class Client{
  
 public:
  Client();
  Client(const char *IP, int portNum, int connfd);
  Client(const char *IP, int portNum);
  ~Client();
  const char *getIP();
  int getPortNum();
  int getConnfd();
  void quit();
  void sign( bool onOrOff );
  void printFiles();
  void makeCache();
  void addClient( Client *client );
  void addFile( File *file );
  void printClients();
  std::vector <File*> *listOfFiles() const {return _listOfFiles;}
  std::vector <Client*> *listOfClients() const {return _listOfClients;}
  char *toChar();
  bool isActive();
  bool isCache();

 private:
  const char *_IPaddr;
  int _portNum;
  int  _connfd;
  bool _active;
  bool _cache;
  char _toChar[15];
  char _buf[1024];

  std::vector <Client*> *_listOfClients;
  std::vector <File*> *_listOfFiles;
  
};

#endif
