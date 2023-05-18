#include <iostream>
#include <string>
#include <vector>

#include "LEXER.h"


using std::endl;
using std::stoi;
using std::stof;
using std::cout;
using std::vector;
using std::string;








Lexer::Lexer(std::string t) {
	text = t;
}


//unsigned int Lexer::hash(const char* v) {
//
//	unsigned int hascode = 0;
//
//	for (int i = 0; i < strlen(v); i++) {
//		hascode += v[i];
//		hascode = (hascode * v[i]);
//	}
//	//std::cout <<  "hash: " << hascode % TABLE_SIZE << std::endl;
//	return (hascode % TABLE_SIZE);
//}

//void Lexer::add(const char* v) {
//	symbols[hash(v)] = v;
//}
//
//const char* Lexer::get(const char* v) {
//	return symbols[hash(v)];
//}
//
void Lexer::printToken(Token* token) {
	cout << token->type << ":";
	cout << token->value;
	cout << '\n';
}
//
//void Lexer::printSymbolTable() {
//	for (int i = 0; i < sizeof(symbols) / 8; i++) {
//		if (symbols[i]) {
//			cout << symbols[i] << endl;
//		}
//	}
//}

bool Lexer::isWhitespace(char c) {
	if (c != 0) {
		if (c == ' ' || c == '\t' || c == '\n') return true;
	}
	else {
		if (CHAR == ' ' || CHAR == '\t' || CHAR == '\n') return true;

	}
	return false;
}

bool Lexer::isKeyword(std::string* s) {
	if (*s == "fn")return true;
	else if (*s == "<-")return true;
	else if (*s == "->")return true;
	else if (*s == "__RET")return true;
	else if (*s == "print")return true;

	return false;
}

bool Lexer::isOperator(char c) {
	char test = CHAR;
	if (c != 0) test = c;
	else if (test == '+') return true;
	else if (test == '-') return true;
	else if (test == '*') return true;
	else if (test == '/') return true;
	else if (test == '%') return true;
	else if (test == '<') return true;
	else if (test == '>') return true;
	else if (test == '=') return true;
	return false;

}

bool Lexer::isSeperator(char c) {
	char test = CHAR;
	if (c != 0) test = c;
	else if (test == '(') return true;
	else if (test == ')') return true;
	else if (test == '[') return true;
	else if (test == ']') return true;
	else if (test == '{') return true;
	else if (test == '}') return true;
	else if (test == ',') return true;
	else if (test == ':') return true;
	else if (test == ';') return true;
	return false;

}

bool Lexer::isSpecial(char c) {
	char test = CHAR;
	if (c != 0) test = c;
	else if (test == '#') return true;
	else if (test == '@') return true;
	else if (test == '?') return true;
	return false;
}

Token* Lexer::makeNumber() {
	std::string num_str;
	int dotCount = 0;
	while (isdigit(text[pos]) || text[pos] == '.') {
		num_str += text[pos];
		if (text[pos] == '.') dotCount++;
		pos++;
	}
	pos--;
	if (dotCount == 0) return new Token{ "CONST i",num_str };
	else if (dotCount == 1) return new Token{ "CONST f",num_str };
	else {
		std::cout << "Error at: Lexer: 'too many dots!'" << pos << '\n';
		exit(0);
	}
	return 0;
}
Token* Lexer::makeIdentifier() {
	std::string str;
	while (isalnum(text[pos]) || text[pos] == '_') {
		str += text[pos];
		pos++;
	}
	pos--;
	
	if (str == "__RET") {
		return new Token{ "KEYWORD", "RETVALUE"};
	}
	else if(isKeyword(&str)) {
		return new Token{ "KEYWORD", str };
	}
	
	return new Token{ "IDENT", str };
}

Token* Lexer::makeString() {
	std::string str;
	pos++;
	while (isalnum(text[pos]) || text[pos] == '"' || isWhitespace(text[pos])) {
		str += text[pos];
		if (text[pos] == '"') break;
		pos++;
	}
	str.pop_back();
	return new Token{ "CONST s",str };
}

Token* Lexer::makeChar() {

	if (pos + 2 < text.size()) {
		if (text[pos + 2] == '\'') {
			/*string s;
			s += text[pos + 1];*/
			Token* tmp = new Token{ "CONST c" };
			tmp->value += text[pos + 1];
			pos += 2;
			return tmp;
		}
		else {
			std::cout << "Error at: Lexer: 'expected apostrophe!'" << pos + 1 << '\n';
			exit(0);
		}
	}
	else {
		std::cout << "Error at: Lexer: 'expected apostrophe!'" << pos + 1 << '\n';
		exit(0);
	}


}

Token* Lexer::makeOperator() {


	if (CHAR == '+') return new Token{ "OPERATOR","PLUS" };
	else if (CHAR == '-') {
		if (pos + 1 < text.size()) {
			if (text[pos + 1] == '>') {
				pos++;
				return new Token{ "KEYWORD","CALL" };
			}
			else {
				return new Token{ "OPERATOR","MINUS" };
			}
		}
	}

	else if (CHAR == '*') return new Token{ "OPERATOR","MUL" };
	else if (CHAR == '/') return new Token{ "OPERATOR","DIV" };
	else if (CHAR == '%') return new Token{ "OPERATOR","MOD" };
	else if (CHAR == '>') return new Token{ "OPERATOR","GREATER" };
	else if (CHAR == '<') {
		if (pos + 1 < text.size()) {
			if (text[pos + 1] == '-') {
				pos++;
				return new Token{ "KEYWORD","RETURN" };
			}
			else {
				return new Token{ "OPERATOR","LESS" };
			}
		}
	}
	else if (CHAR == '=') return new Token{ "OPERATOR","EQUAL" };
	else return 0;
}
Token* Lexer::makeSeperator() {
	if (CHAR == '(') return new Token{ "SEPERATOR","LPAREN" };
	else if (CHAR == ')') return new Token{ "SEPERATOR","RPAREN" };
	else if (CHAR == '[') return new Token{ "SEPERATOR","LBRACKET" };
	else if (CHAR == ']') return new Token{ "SEPERATOR","RBRACKET" };
	else if (CHAR == '{') return new Token{ "SEPERATOR","LBRACE" };
	else if (CHAR == '}') return new Token{ "SEPERATOR","RBRACE" };
	else if (CHAR == ',') return new Token{ "SEPERATOR","COMMA" };
	else if (CHAR == ':') return new Token{ "SEPERATOR","COLON" };
	else if (CHAR == ';') return new Token{ "SEPERATOR","SEMICOLON" };
	else return 0;
}

Token* Lexer::makeSpecial() {
	if (CHAR == '#') return new Token{ "SPECIAL" , "NEWVAR" };
	else if (CHAR == '@') return new Token{ "SPECIAL" , "LOOP" };
	else if (CHAR == '?') return new Token{ "SPECIAL" , "IF" };
	else return 0;
}

Token* Lexer::getNextToken() {
	CHAR = text[pos];
	if (isdigit(CHAR)) {
		return makeNumber();
	}
	else if (isOperator()) {
		return makeOperator();
	}
	else if (isalpha(CHAR) || CHAR == '_') {
		return makeIdentifier();
	}
	else if (CHAR == '"') {
		return makeString();
	}
	else if (CHAR == '\'') {
		return makeChar();
	}
	else if (isSeperator()) {
		return makeSeperator();
	}
	else if (isSpecial()) {
		return makeSpecial();
	}


	else {
		std::cout << "Error at: Lexer: 'unknown character'" << text[pos] << " in int " << (int)text[pos] << '\n';
		//exit(0);
	}
}

//void Lexer::makeSymbolTable() {
//	const char* var =  {0};
//	for (int i = 0; i < tokens.size(); i++) {
//		if (tokens[i]->value == "NEWVAR") {
//			if (i + 1 < tokens.size()) {
//				if (tokens[i + 1]->type == "IDENT") {
//					var = tokens[i + 1]->value.c_str();
//					add(var);
//				}
//			}
//		}
//		else if (tokens[i]->value == "fn") {
//			if (i + 1 < tokens.size()) {
//				if (tokens[i + 1]->type == "IDENT") {
//					var = tokens[i + 1]->value.c_str();
//					add(var);
//				}
//			}
//		}
//	}
//}


void Lexer::printTokens() {
	for (int i = 0; i < tokens.size(); i++) {
		printToken(tokens[i]);
	}
}

void Lexer::lex() {
	while (pos < text.size()) {
		CHAR = text[pos];
		if (isWhitespace()) {
			pos++;
			continue;
		}
		if (CHAR == '/') {
			if (pos + 1 < text.size()) {
				if (text[pos + 1] == '/') {

					while (text[pos] != '\n') {
						pos++;
					}
					continue;
				}
			}
		}


		tokens.push_back(getNextToken());
		//exit(0);
		pos++;
	}
	//makeSymbolTable();
}