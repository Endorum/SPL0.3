#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "utils.h"



#define cval const char *
#define CVAL const char *




using namespace std;


std::string readFile(const char* name, bool debug) {
    std::vector<std::string> filenames;
    std::string text;
    std::ifstream inFile;
    if (name) {
        inFile.open(name);
    }
    else {
        inFile.open("main.sm");
    }
    if (inFile.fail()) {
        // error
        cout << "couln't find file with name: " << name << endl;
        exit(1);
    }


    std::stringstream strStream;
    strStream << inFile.rdbuf();
    text = strStream.str();
    std::cout << "Read file: main.sm" << '\n';
    if (debug) {
        std::cout << "---------------------------------------------------\n";
        std::cout << text << "\n";
        std::cout << "---------------------------------------------------\n";
    }
    std::cout << "Done reading---------------------------------------\n";

    return text;
}

bool isInt(cval input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) return false;
    }
    return true;
}

char charIndex(cval input, int idx) {
    return input[idx];
}

cval cvalIndex(cval input, int idx) {
    if (strlen(input) >= idx) {
        string res(1, input[idx]);
        return _strdup(res.c_str());
    }
    else {
        // error
        cout << "Index error" << endl;
        exit(1);
    }
}

cval charToInt(cval inp) {

    if (strlen(inp) == 1) {
        char c = inp[0];
        int r = (int)c;
        return _strdup(to_string(r).c_str());
    }
    else {
        // error
        cout << "char to int error, input length has to be 1" << endl;
        exit(1);
    }
}

cval strToCVAL(string input) {
    return _strdup(input.c_str());
}

string charToString(char input) {
    return string(1, input);
}

unsigned int hash(cval v) {
    unsigned int hashcode = 0;
    int l = strlen(v);
    int i = 0;
    while (i != l) {
        hashcode += v[i++];
        hashcode += hashcode << 10;
        hashcode ^= hashcode >> 6;

    }
    hashcode += hashcode << 3;
    hashcode ^= hashcode >> 11;
    hashcode += hashcode << 15;


    return (hashcode % TABLE_SIZE_FUNS);
}

bool containsStr(string s, char c) {
    return s.find(c) != std::string::npos;
}
bool containsCval(cval s, char c) {
    return false;
}
bool cmp(cval a, cval b) {
    return strcmp(a, b) == 0;
}