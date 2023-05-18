#pragma once

#include <iostream>
#include <string>
#include <vector>

#define MAX_NAME_SIZE 256
#define TABLE_SIZE 4092

using std::endl;
using std::stoi;
using std::stof;
using std::cout;
using std::vector;
using std::string;



struct Token {
	const char* type;
	std::string value;
};



class Lexer {

public:

	std::string text;

	std::vector<Token*> tokens;

	//const char* symbols[TABLE_SIZE] = { 0 };

	int pos = 0;
	char CHAR = 0;

	Lexer(std::string t);

	void printToken(Token* token);

	/*unsigned int hash(const char* v);

	void add(const char* v);

	const char* get(const char* v);

	void printSymbolTable();*/

	void printTokens();

	bool isWhitespace(char c = 0);

	bool isKeyword(std::string* s);

	bool isOperator(char c = 0);

	bool isSeperator(char c = 0);

	bool isSpecial(char c = 0);

	Token* makeNumber();
	Token* makeIdentifier();

	Token* makeString();

	Token* makeChar();

	Token* makeOperator();
	Token* makeSeperator();

	Token* makeSpecial();

	void makeSymbolTable();

	Token* getNextToken();


	void lex();
};