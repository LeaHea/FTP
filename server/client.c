#include "client.h"
#include <iostream>


Client::Client(){
  _IPaddr = "";
  _portNum = 0;
  _active = 0;
  _cache = 0;
  _listOfClients = new std::vector<Client*>;
  _listOfFiles = new std::vector<File*>;
}

Client::~Client(){
  _IPaddr = "";
  _portNum = 0;
  _active = 0;
  _cache = 0;
  _listOfFiles->clear();
  _listOfClients->clear();

}

Client::Client(const char *IP, int portNum){
}

Client::Client(const char *IP, int portNum, int connfd){
  _IPaddr = IP;
  _portNum = portNum;
  _active = 0;
  _connfd = connfd;
  _cache = 0;
  _listOfClients = new std::vector<Client*>;
  _listOfFiles = new std::vector<File*>;
  
}

void Client::addFile( File *file ){
  _listOfFiles->push_back(file);
}

void Client::printFiles(){
  for( int i = 0; i < _listOfFiles->size(); i++ )
    printf("this is a file: %s\n", listOfFiles()->at(i)->getName());
}

const char *Client::getIP(){
  return _IPaddr;
}

void Client::printClients(){
  for( int i = 0; i < _listOfClients->size(); i++ )
    printf("this is a client: %s\n", listOfClients()->at(i)->toChar());
}

void Client::addClient( Client *client ){
  _listOfClients->push_back(client);
}

void Client::sign( bool onOrOff ){
  _active = onOrOff;
}

void Client::makeCache(){
  _cache = 1;
}

bool Client::isCache(){
  return _cache;
}

char *Client::toChar(){
  sprintf(_toChar, "IP: %s Port: %d\n", _IPaddr, _portNum);
  return _toChar;

}

int Client::getPortNum(){
  return _portNum;
}

int Client::getConnfd(){
  return _connfd;
}

void Client::quit(){
  _active = 0;
}

bool Client::isActive(){
  return _active;
}



