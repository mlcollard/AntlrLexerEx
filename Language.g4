lexer grammar Language;

options {
    language=Cpp;
}

// header of the .h file & .cpp
@lexer::header {// @lexer::header
}

@lexer::preinclude {
    // @lexer::preinclude
}

@lexer::postinclude {
    // @lexer::postinclude
}

@lexer::context {
    // @lexer::context
}

@lexer::members {
    // @lexer::members
    bool inBackTrace = false;
    bool inDef = false;

    // parse
    bool parse(antlr4::BufferedTokenStream& stream);
}

@lexer::declarations {
    // @lexer::declarations

    // (async)? for
    bool parseFor(antlr4::BufferedTokenStream& stream);

    // (async)? def
    bool parseDef(antlr4::BufferedTokenStream& stream);

    // enddef
    bool parseEnddef(antlr4::BufferedTokenStream& stream);

    // return
    bool parseReturn(antlr4::BufferedTokenStream& stream);

    // async (def | for)?
    bool parseAsync(antlr4::BufferedTokenStream& stream);

    // (def | return | for | async)
    bool parseStart(antlr4::BufferedTokenStream& stream);

}

@lexer::definitions {

// parse
bool Language::parse(antlr4::BufferedTokenStream& tokens) {

    while (true) {

        if (tokens.LA(1) == Language::EOF)
            break;

        if (!parseStart(tokens))
            return false;

        std::cout << '\n';
    }

    return true;
}

// (async)? def
bool Language::parseDef(antlr4::BufferedTokenStream& stream) {

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

// (async)? def
bool Language::parseFor(antlr4::BufferedTokenStream& stream) {

    // (ASYNC)?
    if (stream.LA(1) == Language::ASYNC) {
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
    }

    // For
    if (stream.LA(1) == Language::FOR) {
        inDef = true;
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// enddef
bool Language::parseEnddef(antlr4::BufferedTokenStream& stream) {

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
bool Language::parseReturn(antlr4::BufferedTokenStream& stream) {

   if (stream.LA(1) == Language::RETURN) {
        if (!inBackTrace)
            std::cout << stream.LT(1)->toString();
        stream.consume();
        return true;
    }

    return false;
}

// async (def | for)?
bool Language::parseAsync(antlr4::BufferedTokenStream& stream) {

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
bool Language::parseStart(antlr4::BufferedTokenStream& stream) {

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

}
DEF : 'def' ;

ENDDEF : 'enddef' ;

ASYNC : 'async' ;

FOR : 'for' ;

RETURN : 'return' ;

SEMICOLON : ';' ;

EXPR : [A-Z] ;

LINECOMMENT : '/' '/' .*? '\n' ;

BLOCKCOMMENT : '/' '*' .*? '*' '/' ;

REST : .+? -> skip ;
