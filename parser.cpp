/*
    identity.cpp

    Identity copy
*/

#include <iostream>
#include <fstream>
#include "Language.h"

bool inBackTrace = false;
bool inDef = false;

// (async)? for
bool parseFor(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // FOR
    if (stream.LA(1) == Language::FOR) {
        if (!inBackTrace)
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
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // DEF
    if (stream.LA(1) == Language::DEF) {
        inDef = true;
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// enddef
bool parseEnddef(antlr4::BufferedTokenStream& stream) {

    // ENDDEF
    if (stream.LA(1) == Language::ENDDEF) {
        inDef = false;
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// return
bool parseReturn(antlr4::BufferedTokenStream& stream) {

   if (stream.LA(1) == Language::RETURN) {
        if (!inBackTrace)
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
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// (def | return | for | async)
bool parseStart(antlr4::BufferedTokenStream& stream) {

    if (!inDef && stream.LA(1) == Language::DEF) {
        return parseDef(stream);
    } else if (inDef && stream.LA(1) == Language::ENDDEF) {
        return parseEnddef(stream);
    } else if (stream.LA(1) == Language::RETURN) {
        return parseReturn(stream);
    } else if (stream.LA(1) == Language::FOR) {
        return parseFor(stream);
    } else if (stream.LA(1) == Language::ASYNC) {

        inBackTrace = true;

        // mark
        auto pos = stream.index();

        // ( (ASYNC)? FOR )=> (ASYNC)? FOR
        if (parseFor(stream)) {
            // really parse For
            inBackTrace = false;

            // rewind
            stream.seek(pos);

            return parseFor(stream);
        }

        // rewind
        stream.seek(pos);

        // { !inDef }? ( (ASYNC)? DEF )=> (ASYNC)? DEF
        if (!inDef && parseDef(stream)) {
            // really parse For
            inBackTrace = false;

            // rewind
            stream.seek(pos);

            return parseDef(stream);
        }

        // ASYNC
        // really parse Async
        inBackTrace = false;

        // rewind
        stream.seek(pos);

        return parseAsync(stream);

        // return parseDef(stream);

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
