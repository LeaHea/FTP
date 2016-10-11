#ifndef _FILE
#define _FILE
#include <string>

class File{

 public:
  File(char *name);
  ~File();
  char * getName();
  char * getPath();

 private:
  char * _name;
  char * _path;

};

#endif
