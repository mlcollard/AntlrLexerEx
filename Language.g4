lexer grammar Language;

options {
    language=Cpp;
}

DEF : 'def' ;

ASYNC : 'async' ;

FOR : 'for' ;

RETURN : 'return' ;

SEMICOLON : ';' ;

EXPR : [A-Z] ;

LINECOMMENT : '/' '/' .*? '\n' ;

BLOCKCOMMENT : '/' '*' .*? '*' '/' ;

REST : .+? -> skip ;
