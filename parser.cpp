/*
    identity.cpp

    Identity copy
*/

#include <iostream>
#include <fstream>
#include "Language.h"

int main(int argc, char* argv[]) {

    // setup input
    std::ifstream file(argv[1]);
    antlr4::ANTLRInputStream input(file);

    // setup lexer
    Language lexer(&input);
    antlr4::BufferedTokenStream tokens(&lexer);

    // // run lexer
    // tokens.fill();
    // for (auto token : tokens.getTokens()) {
    //     std::cout << token->getText();
    // }

    return 0;
}
