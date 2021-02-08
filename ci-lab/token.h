/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * token.h - This file contains the enum of possible token types, as well as
 * the lexeme struct used by the lexer.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#define MAX_LINE_CHARS 82

/* This enum contains all possible types a token may have. */
typedef enum {
    TOK_ID,             // identifier
    // literals
    TOK_NUM,            // 0-9
    TOK_TRUE,           // true
    TOK_FALSE,          // false
    TOK_STR,            // "..."
    // delimiters
    TOK_LPAREN,         // (
    TOK_RPAREN,         // )
    // ternary operator
    TOK_QUESTION,       // ?
    TOK_COLON,          // :
    // binary operators
    TOK_PLUS,           // +
    TOK_BMINUS,         // -
    TOK_TIMES,          // *
    TOK_DIV,            // /
    TOK_MOD,            // %
    TOK_AND,            // &
    TOK_OR,             // |
    TOK_LT,             // <
    TOK_GT,             // >
    TOK_EQ,             // ~
    //unary operators
    TOK_UMINUS,         // _
    TOK_NOT,            // !
    // other stuff
    TOK_SEP,            // format separator
    TOK_EOL,            // end of line
    TOK_ASSIGN,         // =
    TOK_IDENTITY,       // do nothing
    TOK_FMT_SPEC,       // format specifier: needs to be disambiguated from 
                        // identifier or Boolean literals by parser
    TOK_INVALID = -1    // sentinel
} token_t;

/* This struct is used by the lexer to provide information about the tokens
 * within an input string. */
typedef struct lexeme {
    token_t ttype;              // the type of the token
    int startpos;               // the position of the token in the input
    char repr[MAX_LINE_CHARS];  // the representation of the token
} lexeme_t, *lptr_t;
