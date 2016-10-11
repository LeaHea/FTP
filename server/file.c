#include "file.h"

File::File(char * name){
  _name = name;
  _path = name;

}

File::~File(){

}

char * File::getName(){
  return _name;

}

char * File::getPath(){
  return _path;
}
