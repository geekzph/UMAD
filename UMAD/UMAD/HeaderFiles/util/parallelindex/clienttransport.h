#ifndef CLIENTTRANSPORT_H
#define CLIENTTRANSPORT_H

#include "umadclient.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::fstream;
using std::stringstream;

extern void s_pipfun(int);
extern void joinCharArray(char*&dest,const char *sor);

//---------------------VECTOR---------------------------//
extern int sendvectorquerydata(int *, int, char *, int);
extern int sendvectorrawdata(int *, int, char *, int);
//---------------------VECTOR---------------------------//

#endif