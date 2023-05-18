#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <numeric>
#include <chrono>


#include "UTILS.h"
#include "LEXER.h"
#include "INTERPRETER.cpp"


unsigned int hashNew(const char*n) {
    unsigned int hashcode = 0;

    for (int i = 0; i < strlen(n); i++) {
        hashcode = hashcode * 33 + n[i];
    }
    return hashcode % TABLE_SIZE;
}


int main(int argc,const char ** argv)
{

    std::string program = readFile();

    Lexer lexer(program);
    

    lexer.lex();

    //lexer.printSymbolTable();


    Interpreter interpreter(lexer.tokens);
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::\n";

    interpreter.interpret(); 
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::\n";

      
    cout << "Functions: " << endl;
    interpreter.printFunctions();
    cout << "###################################################\n";

    cout << "Variables: " << endl;
    interpreter.printVarTable();
    
    cout << "Stack: " << endl;
    interpreter.printStack();

    return 0;
}
