/*
    trace.cpp

    Trace the tokens
*/

#include <iostream>
#include <fstream>
#include "Comments.h"

int main(int argc, char* argv[]) {

    // setup input
    std::ifstream file(argv[1]);
    antlr4::ANTLRInputStream input(file);

    // setup lexer
    Comments lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    // run lexer
    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << '\n';
    }

    return 0;
}
