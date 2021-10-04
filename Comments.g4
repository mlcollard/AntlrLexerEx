lexer grammar Comments;

LINECOMMENT : '/' '/' .*? '\n' ;

BLOCKCOMMENT : '/' '*' .*? '*' '/' ;

REST : .+?;
