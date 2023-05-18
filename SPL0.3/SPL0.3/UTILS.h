#pragma once
#include <string>
#include <iostream>
#include <vector>


#define cval const char *
#define CVAL const char *


#define TABLE_SIZE_VARS 65536 // 2^20 ~ 
#define TABLE_SIZE_FUNS 65536 // 2^20 ~ 

using std::endl;
using std::stoi;
using std::stof;
using std::cout;
using std::vector;
using std::string;

std::string readFile(const char* name = 0, bool debug = false);
char charIndex(cval input, int idx);
cval cvalIndex(cval input, int idx);
cval charToInt(cval inp);
cval strToCVAL(string input);
string charToString(char input);
unsigned int hash(cval v);
bool containsStr(string s, char c);
bool cmp(cval a, cval b);