/*
    identity.cpp

    Identity copy
*/

#include <iostream>
#include <fstream>
#include "Language.h"

// (async)? for
void parseFor(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // FOR
    if (stream.LA(1) == Language::FOR) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    } else {
        std::cerr << "ERROR\n";
        exit(1);
    }
}

// (async)? def
void parseDef(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // DEF
    if (stream.LA(1) == Language::DEF) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    } else {
        std::cerr << "ERROR\n";
        exit(1);
    }
}

// return
void parseReturn(antlr4::BufferedTokenStream& stream) {

   if (stream.LA(1) == Language::RETURN) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    } else {
        std::cerr << "ERROR\n";
        exit(1);
    }
}

// async (def | for)?
void parseAsync(antlr4::BufferedTokenStream& stream) {

    // async
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    } else {
        std::cerr << "ERROR\n";
        exit(1);
    }

    // // (def | for)?
    // if (stream.LA(1) == Language::DEF) {
    //     parseDef(stream);
    // } else if (stream.LA(1) == Language::FOR) {
    //     parseFor(stream);
    // }
}

// (def | return | for | async)
void parseStart(antlr4::BufferedTokenStream& stream) {

    if (stream.LA(1) == Language::DEF) {
        parseDef(stream);
    } else if (stream.LA(1) == Language::RETURN) {
        parseReturn(stream);
    } else if (stream.LA(1) == Language::FOR) {
        parseFor(stream);
    } else if (stream.LA(1) == Language::ASYNC && stream.LA(2) == Language::FOR) {
        parseFor(stream);
    } else if (stream.LA(1) == Language::ASYNC && stream.LA(2) == Language::DEF) {
        parseDef(stream);
    } else if (stream.LA(1) == Language::ASYNC) {
        parseAsync(stream);
    } else {
        std::cerr << "ERROR\n";
        exit(1);
    }
}

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

        parseStart(tokens);
        std::cout << '\n';
    }

    return 0;
}
