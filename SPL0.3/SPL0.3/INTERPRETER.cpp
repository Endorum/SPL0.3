#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "UTILS.h"
#include "LEXER.h"

struct Node {
	const char* type;
	const char* value;
};

struct Variable {
	const char* name;
	const char* value=0;
};

struct Function {
	const char* name=0;
	int start=0;
	int end=0;
	int returnAddress=0;
	Function* prevScope = 0;
	std::vector<Variable*> args;
	std::vector<Node*> body;
};

class Interpreter {
public:

	int pos = 0;

	Token TOK;

	std::vector<Token*> tokens;
	std::vector<Node*> nodes;

	Function* scope = 0;
	Function* prevScope = 0;

	bool BooleanResult = false;

	Variable* vars[TABLE_SIZE] = { 0 };
	Function* funs[TABLE_SIZE] = { 0 };

	std::vector<Variable*> varStack;
	std::vector<const char*> stack;

	int varStackPointer = 0;
	
	
	
	Node* node = 0;

	const char* returnValue="0";

	Interpreter(std::vector<Token*> t) {
		tokens = t;
		TOK = *tokens[pos];
		makeNodes();
	}

	// hashing

	unsigned int hash(const char * v) {
		unsigned int hashcode = 0;

		for (int i = 0; i < strlen(v); i++) {
			hashcode += v[i];
			hashcode = (hashcode * v[i]);
		}
		return (hashcode % TABLE_SIZE);
	}

	Variable* getVar(const char * n) {
		Variable* res = vars[hash(n)];
		if (res) return res;
		else {
			if (scope) {
				for (int i = 0; i < scope->args.size(); i++) {
					if (streq(n, scope->args[i]->name)) {
						return scope->args[i];
					}
				}
				cout << "couldn't find variable: " << n << " (are in scope) " << '\n';
				exit(0);
			}
			else {
				cout << "couldn't find variable: " << n << " (not in scope) " << '\n';
				exit(0);
			}
			
		}
	}

	void addVar(Variable* v) {
		vars[hash(v->name)] = v;
	}
	
	Function* getFunc(const char * n) {
		Function* res = funs[hash(n)];
		if (res) return res;
		else {
			cout << "couldn't find function: " << n << '\n';
			exit(0);
		}
	}

	void addFunc(Function* f) {
		funs[hash(f->name)] = f;
	}

	// hashing end

	void pushVar(Variable* v) {
		varStackPointer++;
		varStack.push_back(v);
	}
	
	Variable* popVar() {
		varStackPointer--;
		Variable* res = varStack.back();
		varStack.pop_back();
		return res;
	}

	void push(const char* v) {
		stack.push_back(v);
	}

	const char* pop() {
		const char* ret = stack.back();
		stack.pop_back();
		return ret;
	}

	void makeNodes() {
		Node* node=0;
		for (int i = 0; i < tokens.size(); i++) {
			node = new Node{ tokens[i]->type, tokens[i]->value.c_str() };
			nodes.push_back(node);
		}
	}


	void peek() {
		node = nodes[pos];
	}

	void next() {
		if (pos+1<nodes.size()) {
			pos++;
			node = nodes[pos];
			
		}
		else {
			node = 0;
		}
		
	}
	
	bool isConst(const char* c) {
		if (c == "CONST i" || c == "CONST f" || c == "CONST c" || c == "CONST s") return 1;
		return 0;
	}

	bool streq(const char * str1, const char * str2) {
		return strcmp(str1, str2) == 0;
	}

	/*int toInt(const char * n, int value=0) {
		
		for (int i = 0; i < strlen(n); i++) {
			if (!isdigit(n[i])) {
				return value;
			}
		}
		return stoi(n);
	}*/
	double toFloat(const char * n, int value=0) {
		
		for (int i = 0; i < strlen(n); i++) {
			if (!(isdigit(n[i]) || n[i] == '.')) {
				return value;
			}
		}
		return std::stod(n);
	}

	bool isString(const char * s) {
		return (!isdigit(s[0]));
	}

	void printNodes(std::vector<Node*> * n=0) {
		if(!n){
			for (int i = 0; i < nodes.size(); i++) {
				cout << nodes[i]->type << ":" << nodes[i]->value << endl;
			}
		}
		else {
			for (int i = 0; i < (*n).size(); i++) {
				cout << (*n)[i]->type << ":" << (*n)[i]->value << endl;
			}
		}
		cout << endl;
	}

	void printVarTable() {
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (vars[i]) {
				cout << vars[i]->name << " ";
				if (vars[i]->value) {
					cout << vars[i]->value;
				}
				cout << " Index: " << i << '\n';
			}
		}
	}

	void printStack() {
		for (int i = 0; i < stack.size(); i++) {
			cout << stack[i] << " ";
		}
		cout << endl;
	}

	void printScope() {
		printFunction(scope);
	}

	void printFunction(Function* fn) {
		cout << "---------------------------------------------------\n";
		
		cout << "name:" << fn->name << endl;
		cout << "\nStart: " << fn->start << " End: " << fn->end << " Return to: " << fn->returnAddress << endl;
		cout << "args: " << endl;
		for (int i = 0; i < fn->args.size(); i++) {
			cout << fn->args[i]->name << " ";

			if (fn->args[i]->value) {
				cout << ":" << fn->args[i]->value << " ";
			}
		}
		cout << "\n\nBody: \n" << endl;
		printNodes(&fn->body);
		

		
	}

	void printFunctions() {
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (funs[i]) {
				printFunction(funs[i]);
				cout << "Table Index: " << i << '\n';
			}
		}
	}

	bool checkNextType(const char* c) {
		if (pos + 1 < nodes.size()) {
			if (streq(c, "CONST")) {
				pos++;
				peek();
				return isConst(node->type);
			}
			else {
				pos++;
				peek();
				return cmp(node->type, c);
			}
		}
		return 0;
		
	}

	bool checkNextValue(const char * c) {
		if (pos + 1 < nodes.size()) {
			pos++;
			peek();
			return streq(node->value, c);
		}
		return 0;
	}
	bool checkCurrType(const char* c) {
		if (streq(c, "CONST")) {
			peek();
			return isConst(node->type);
		}
		else {
			peek();
			return streq(node->type, c);
		}
	}

	void advanceBack(int length = 1) {
		if (pos - length >= 0) {
			pos -= length;
			peek();
		}
		else {
			cout << "advanced to much back" << pos << endl;
			exit(0);
		}
	}

	bool checkCurrValue(const char * c) {
		peek();
		return streq(node->value, c);
	}

	void advance(int length = 1) {
		if (pos + length < nodes.size()) {
			pos += length;
			peek();
		}
		else {
			cout << "advanced to much forward" << pos << endl;
			exit(0);
		}
	}

	int interSingle(bool inScope=false,int p=0) {
		int startPos = pos;
		int prevPos = pos;
		int expressionLength = 1;
		if (p != 0) pos = p;

		if (inScope) {
			if (checkCurrValue("NEWVAR") && checkNextType("IDENT")) {
				cout << "#<ident> ";
				prevPos = pos;
				expressionLength++;
				Variable* var = new Variable;
				var->name = node->value;

				//string varvalue;

				if (checkNextValue("EQUAL") && checkNextType("CONST")) {
					cout << "= <const>";
					var->value = node->value;
					expressionLength += 2;
				}
				pos = prevPos;
				if (checkNextValue("EQUAL") && checkNextType("IDENT")) {
					cout << "= <ident>";
					Variable* tmp = getVar(node->value);
					var->value = 0;
					if (tmp) {
						var->value = tmp->value;
					}
					else {
						cout << "couldn't find variable: " << node->value << '\n';
						exit(0);
					}
					prevPos = pos;
					expressionLength += 2;
					if (checkNextValue("LBRACKET") && checkNextType("CONST") && checkNextValue("RBRACKET")) {
						cout << "[<const>]";
						advanceBack();
						if (streq(node->type, "CONST i")) {

							if (strlen(tmp->value) > stoi(node->value)) {
								string s = tmp->value;
								string res(1, s[stoi(node->value)]);
								var->value = _strdup(res.c_str());
							}
							else {
								cout << "Index error: string length:" << strlen(tmp->value) << " less than index: " << stoi(node->value) << endl;
								exit(0);
							}
						}
						expressionLength += 2;
					}
					pos = prevPos;
					if (checkNextValue("LBRACKET") && checkNextType("IDENT") && checkNextValue("RBRACKET")) {
						cout << "[<ident>]";
						advanceBack();
						int idx = stoi(getVar(node->value)->value);
						if (strlen(tmp->value) > idx) {
							string s = tmp->value;
							string res(1, s[idx]);
							var->value = _strdup(res.c_str());
						}
						else {
							cout << "Index error: string length:" << strlen(tmp->value) << " less than index: " << idx << endl;
							exit(0);
						}

						expressionLength += 2;
					}
				}
				pos = prevPos;
				if (checkNextValue("EQUAL") && checkNextValue("RETVALUE")) {
					cout << "= __RET";
					advanceBack(2);
					var->value = returnValue;
					expressionLength += 1;

				}
				addVar(var);
			}
			pos = startPos;
			prevPos = pos;
			if (checkCurrType("IDENT") && checkNextType("OPERATOR")) {
				cout << "<ident> <operator:" << node->value << "> ";
				prevPos = pos;
				expressionLength++;
				const char* op = node->value;
				advanceBack();
				Variable* var = getVar(node->value);
				const char* leftSide = var->value;
				const char* rightSide = 0;
				advance();

				if (checkNextType("CONST")) {
					cout << "<const>";
					rightSide = node->value;

					expressionLength++;
				}
				pos = prevPos;
				if (checkNextType("IDENT")) {
					cout << "<ident>";
					expressionLength++;
					prevPos = pos;
					Variable* tmp = getVar(node->value);
					rightSide = tmp->value;
					if (checkNextValue("LBRACKET") && checkNextType("CONST") && checkNextValue("RBRACKET")) {
						cout << "[<const>]";
						advanceBack();
						if (streq(node->type, "CONST i")) {

							if (strlen(tmp->value) > stoi(node->value)) {
								string s = tmp->value;
								string res(1, s[stoi(node->value)]);
								rightSide = _strdup(res.c_str());
							}
							else {
								cout << "Index error: string length:" << strlen(tmp->value) << " less than index: " << stoi(node->value) << endl;
								exit(0);
							}
						}
						expressionLength += 3;
					}
					pos = prevPos;
					if (checkNextValue("LBRACKET") && checkNextType("IDENT") && checkNextValue("RBRACKET")) {
						cout << "[<ident>]";
						advanceBack();
						if (streq(node->type, "IDENT")) {

							if (strlen(tmp->value) > stoi(getVar(node->value)->value)) {
								string s = tmp->value;
								string res(1, s[stoi(getVar(node->value)->value)]);
								rightSide = _strdup(res.c_str());
							}
							else {
								cout << "Index error: string length:" << strlen(tmp->value) << " less than index: " << stoi(getVar(node->value)->value) << endl;
								exit(0);
							}
						}
						expressionLength += 3;
					}

				}
				pos = prevPos;
				if (checkNextValue("RETVALUE")) {
					cout << "__RET";
					rightSide = returnValue;
					expressionLength++;

				}
				if (streq(op, "PLUS")) {
					getVar(var->name)->value = _strdup((std::to_string((double)(toFloat(leftSide) + toFloat(rightSide)))).c_str());
				}
				else if (streq(op, "MINUS")) {
					getVar(var->name)->value = _strdup((std::to_string((double)(toFloat(leftSide) - toFloat(rightSide)))).c_str());
				}
				else if (streq(op, "MUL")) {
					getVar(var->name)->value = _strdup((std::to_string((double)(toFloat(leftSide) * toFloat(rightSide)))).c_str());
				}
				else if (streq(op, "DIV")) {
					getVar(var->name)->value = _strdup((std::to_string((double)(toFloat(leftSide) / toFloat(rightSide)))).c_str());
				}
				else if (streq(op, "MOD")) {
					getVar(var->name)->value = _strdup((std::to_string(((int)(toFloat(leftSide)) % (int)(toFloat(rightSide))))).c_str());
				}
				else if (streq(op, "EQUAL")) {
					getVar(var->name)->value = rightSide;
				}
				if (isString(leftSide)) {
					if (rightSide)
						if (!isdigit(rightSide[0])) {
							string s = leftSide;

							if (rightSide) s += rightSide;

							var->value = _strdup(s.c_str());

						}
				}
			}
			pos = startPos;
			prevPos = pos;
			if (checkCurrValue("print")) {

				string printValue;
				prevPos = pos;
				if (checkNextType("CONST") && checkNextValue("nl")) {
					advanceBack();
					printValue = node->value;
					printValue += '\n';
				}
				pos = prevPos;
				if (checkNextType("IDENT") && checkNextValue("nl")) {
					advanceBack();
					printValue = getVar(node->value)->value;
					printValue += '\n';
				}
				pos = prevPos;
				if (checkNextType("CONST")) {
					printValue = node->value;
				}
				pos = prevPos;
				if (checkNextType("IDENT")) {
					printValue = getVar(node->value)->value;
				}
				cout << printValue;
			}
			pos = startPos;
			prevPos = pos;
			pos = startPos;
			if (checkCurrValue("CALL") && checkNextType("IDENT") && checkNextValue("LPAREN")) {
				prevPos = pos;
		
				// getting the function from hashtable by name
				advanceBack();
				Function * fn = getFunc(node->value);
				if (fn) {

				}
				else {
					cout << "couln't find function with name: " << node->value << endl;
					exit(0);
				}
				advance();
				std::vector<const char*> args;

				// passing arguments as const char * (only the values!)
				while (!checkNextValue("RPAREN")) {
					if (checkCurrType("IDENT")) {
						args.push_back(getVar(node->value)->value);
					}
					if (checkCurrType("CONST")) {
						args.push_back(node->value);
					}
				}
				if (fn->args.size() == args.size()) {
					for (int i = 0; i < args.size(); i++) {
						fn->args[i]->value = _strdup(args[i]);
					}
				}
				else {
					cout << "amount of arguments not aligned " << "expected: " << fn->args.size() << " got: " << args.size() << endl;
					exit(0);
				}
				// saving return address to stack like in assembly
				push(_strdup((std::to_string(pos+1)).c_str()));
				prevScope = scope;
				scope = fn;
				pos = scope->start;
				
				interpretScope();

 			}
			pos = startPos;
			prevPos = pos;
			if (checkCurrValue("RETURN")) {
				expressionLength++;
				if (scope) {
					prevPos = pos;
					if (checkNextType("IDENT")) {
						expressionLength++;
						returnValue = getVar(node->value)->value;
					}
					pos = prevPos;
					if (checkNextType("CONST")) {
						expressionLength++;
						returnValue = node->value;
					}
					scope = prevScope;
					pos = stoi(pop());
					cout << " returning the value: " << returnValue << endl;
 				}
				else {
					cout << "can't return if not in scope!" << endl;
					exit(0);
				}
			}

			pos = startPos;
			prevPos = pos;
			if (checkCurrValue("LPAREN")) {
				const char* leftSide = 0;
				const char* op = 0;
				const char* rightSide = 0;

				
				advance();
				if (checkCurrType("CONST")) {
					leftSide = node->value;
				}
				else if (checkCurrType("IDENT")) {
					leftSide = getVar(node->value)->value;
				}
				else {
					cout << "expected constant or variable got: " << node->type << ":" << node->value << endl;
					exit(0);
				}
				advance();
				if (checkCurrType("OPERATOR")) {
					if (checkCurrValue("GREATER") || checkCurrValue("EQUAL") || checkCurrValue("LESS")) {
						op = node->value;
					}
					else {
						cout << "expected logical operator got: " << node->type << ":" << node->value << endl;
						exit(0);
					}
				}
				else {
					cout << "expected logical operator got: " << node->type << ":" << node->value << endl;
					exit(0);
				}
				advance();
				if (checkCurrType("CONST")) {
					rightSide = node->value;
				}
				else if (checkCurrType("IDENT")) {
					rightSide = getVar(node->value)->value;
				}
				else {
					cout << "expected constant or variable got: " << node->type << ":" << node->value << endl;
					exit(0);
				}
				advance();
				if (checkCurrValue("RPAREN")) {
					BooleanResult = false;
					if (streq(op, "GREATER") && (std::stod(leftSide) > std::stod(rightSide))) BooleanResult = true;
					if (streq(op, "EQUAL") && (std::stod(leftSide) == std::stod(rightSide))) BooleanResult = true;
					if (streq(op, "LESS") && (std::stod(leftSide) < std::stod(rightSide))) BooleanResult = true;
				}
				else {
					cout << "expected right parenthesis got: " << node->type << ":" << node->value << endl;
					exit(0);
				}
				expressionLength = 5;
			}

			pos = startPos;
			prevPos = pos;
			if (checkCurrValue("IF")) {
				Function* ifTrue = new Function{ "ifTrue" };
				Function* ifFalse = new Function{ "ifFalse" };
				if (checkNextValue("LBRACE")) {
					ifTrue->start = pos;
					while (!checkNextValue("RBRACE")) {
						ifTrue->body.push_back(node);
					}
					ifTrue->end = pos;
					ifTrue->returnAddress = pos + 1;

					if (checkNextValue("COLON")) {
						ifFalse->start = pos;
						if (checkNextValue("LBRACE")) {
							while (!checkNextValue("RBRACE")) {
								ifFalse->body.push_back(node);
							}
						}
						ifFalse->end = pos;
						ifTrue->returnAddress = pos + 1;
						ifFalse->returnAddress = pos + 1;

					}
				}
				
				
			}

		}
		pos = startPos;
		prevPos = pos;
		
		if (checkCurrValue("fn") && checkNextType("IDENT")) {
			Function* fn = new Function;
			Variable* arg = new Variable{ "","" };

			fn->name = node->value;
			
			while (!checkNextValue("LBRACE")) {
				if (checkCurrType("IDENT")) {
					arg = new Variable{ node->value,"0" };
					fn->args.push_back(arg);
				}
			}
			
			fn->start = pos + 1;
			int scopes = 1;
			while (!(scopes==0)) {
				advance();
				if (checkCurrValue("LBRACE")) scopes++;
				if (checkCurrValue("RBRACE")) scopes--;
				fn->body.push_back(node);
			}
			fn->body.pop_back();
			fn->end = pos;
			addFunc(fn);
			
		}
		

		pos = startPos;
		prevPos = pos;
		cout << '\n'; // <- remove when all the <ident> <..> <...> ... etc are gone
		//cout << " length: " << expressionLength << '\n';
		return pos - startPos + 1;;
	}

	

	void interpretScope() {
		pos = scope->start;
		while (pos < scope->end) {
			peek();
			pos += interSingle(true);
		}
	}

	void interpret() {
		while (pos<nodes.size()) {
			peek();
			pos += interSingle();
		}
		scope = funs[hash("main")];
		if (scope) {
			interpretScope();
		}
		else {
			cout << "couldn't find a main function" << endl;
			exit(0);
		}
	}

};