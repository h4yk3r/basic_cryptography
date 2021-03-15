#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace ::std;

string readhex_up (string infilename);
string readhex_low (string infilename);
string readbin (string infilename);
void writehex_up (string outfilename, string ss);
void writehex_low (string outfilename, string ss);
void writebin (string outfilename, string ss);

#endif
