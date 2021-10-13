/*
    lexerapp.cpp

    Lexes
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
    lexer.parse(tokens);

    // output any remaining tokens
    while (tokens.LA(1) != Language::EOF) {
        std::cerr << "Remaining Tokens:\n";
        std::cerr << tokens.LT(1)->toString() << '\n';
        tokens.consume();
    }

    return 0;
}
