#ifndef SERVERTRANSPORT_H
#define SERVERTRANSPORT_H

#include "umadserver.h"

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

extern void s_fun(int);

//-------------------------DATA TRANSPORTATION FUNCTION SERVER SIDE---------------------------//
extern void recvvectorforbuildindex(int, string &, int, int);
extern void recvvectorforsearching(int, string &, int, int);
//--------------------------------------------------------------------------------------------//

#endif