/*
    identity.cpp

    Identity copy
*/

#include <iostream>
#include <fstream>
#include "Language.h"

// (async)? for
bool parseFor(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // FOR
    if (stream.LA(1) == Language::FOR) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// (async)? def
bool parseDef(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // DEF
    if (stream.LA(1) == Language::DEF) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// return
bool parseReturn(antlr4::BufferedTokenStream& stream) {

   if (stream.LA(1) == Language::RETURN) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// async (def | for)?
bool parseAsync(antlr4::BufferedTokenStream& stream) {

    // async
    if (stream.LA(1) == Language::ASYNC) {
        std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// (def | return | for | async)
bool parseStart(antlr4::BufferedTokenStream& stream) {

    if (stream.LA(1) == Language::DEF) {
        return parseDef(stream);
    } else if (stream.LA(1) == Language::RETURN) {
        return parseReturn(stream);
    } else if (stream.LA(1) == Language::FOR) {
        return parseFor(stream);
    } else if (stream.LA(1) == Language::ASYNC && stream.LA(2) == Language::FOR) {
        return parseFor(stream);
    } else if (stream.LA(1) == Language::ASYNC && stream.LA(2) == Language::DEF) {
        return parseDef(stream);
    } else if (stream.LA(1) == Language::ASYNC) {
        return parseAsync(stream);
    } else {
        return false;
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

        if (!parseStart(tokens))
            break;

        std::cout << '\n';
    }

    // output any remaining tokens
    while (tokens.LA(1) != Language::EOF) {
        std::cerr << "Remaining Tokens:\n";
        std::cerr << tokens.LT(1)->toString() << '\n';
        tokens.consume();
    }

    return 0;
}
