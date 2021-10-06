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

    // parse
    while (true) {

        if (tokens.LA(1) == Language::EOF)
            break;

        tokens.consume();
    }

    return 0;
}
