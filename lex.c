/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * lex.c - The lexical analysis module for the interpreter.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"
#include "ansicolors.h"

FILE *infile = NULL;
lptr_t this_token, next_token;

extern void finalize(void);

static char input_line[MAX_LINE_CHARS];
static int lptr;
static char token[MAX_LINE_CHARS];
static lexeme_t lex_array[2];
static char printbuf[100];

static const char CMD_START_CHAR = '@';
static const char STRING_DELIMITER_CHAR = '\"';

static const char *VALID_SCTS = "()?:+-*/%&|<>~_!#\n=";
static const int NUM_SCTS = TOK_ASSIGN - TOK_LPAREN + 1;

static const struct {
    char c;
    token_t t;
} single_char_tokens[] = {
    {'(', TOK_LPAREN},
    {')', TOK_RPAREN},
    {'?', TOK_QUESTION},
    {':', TOK_COLON},
    {'+', TOK_PLUS},
    {'-', TOK_BMINUS},
    {'*', TOK_TIMES},
    {'/', TOK_DIV},
    {'%', TOK_MOD},
    {'&', TOK_AND},
    {'|', TOK_OR},
    {'<', TOK_LT},
    {'>', TOK_GT},
    {'~', TOK_EQ},
    {'_', TOK_UMINUS},
    {'!', TOK_NOT},
    {'#', TOK_SEP},
    {'\n', TOK_EOL},
    {'=', TOK_ASSIGN}
};

static token_t check_SCT(char c) {
    for (int i = 0; i < NUM_SCTS; i++)
        if (single_char_tokens[i].c == c) return single_char_tokens[i].t;
    return TOK_INVALID;
}

static void get_token(lptr_t lexp) {
    // check running status
    if (terminate || ignore_input) return;

    int c;
    token_t t;
    char *s = lexp->repr;
    // skip whitespace
    while ((c = input_line[lptr]) == ' ' || c == '\t') lptr++;
    lexp->startpos = lptr;
    
    // handle any commands
    if (CMD_START_CHAR == c) {
        switch (c = input_line[++lptr]) {
            case 'q':
                terminate = true;
                break;
            case 'p':
                print_table();
                ignore_input = true;
                break;
            default:
                handle_error(ERR_LEX);
                break;
        }
        return;
    }
    
    // handle single character tokens
    if ((t = check_SCT(c)) != TOK_INVALID) {
        lptr++;
        lexp->ttype = t;
        return;
    }
    int i = -1;
    
    // handle identifiers
    if (isalpha(c)) {
        while (isalnum(s[++i] = c = input_line[lptr++]));
        s[i] = '\0';
        lptr--;
        lexp->ttype = TOK_ID;
        return;
    }
    
    // handle integer constants
    if (isdigit(c)) {
        while (isdigit(s[++i] = c = input_line[lptr++]));
        s[i] = '\0';
        lptr--;
        lexp->ttype = TOK_NUM;
        return;
    }

    // handle strings
    if (STRING_DELIMITER_CHAR == c) {
        c = input_line[++lptr];
        lexp->ttype = TOK_STR;
        if (c == STRING_DELIMITER_CHAR) {
            s[0] = '\0';
            lptr++;
            return;
        }
        while (c != '\0' && c != STRING_DELIMITER_CHAR) s[++i] = c = input_line[lptr++];
        if(c != STRING_DELIMITER_CHAR) {
            handle_error(ERR_LEX);
            return;
        }
        s[i] = '\0';
        return;
    }

    handle_error(ERR_LEX);
}

void init_lexer(void) {
    if (fgets(input_line, sizeof(input_line), infile) == NULL) {
        logging(LOG_WARNING, "interpreter exited without @q");
        terminate = true;
        return;
    }
    if (strchr(input_line, '\n') == NULL) {
        if (strlen(input_line) >= MAX_LINE_CHARS - 1) {
            sprintf(printbuf, "max input size is %d characters", MAX_LINE_CHARS - 2);
            logging(LOG_ERROR, printbuf);
            while ((input_line[0] = fgetc(infile)) != '\n' && input_line[0] != EOF);
        } else {
            logging(LOG_ERROR, "expression ends without newline");
        }
        return;
    }

    lptr = 0;
    this_token = lex_array;
    next_token = lex_array+1;
    get_token(this_token);
    get_token(next_token);
}

void advance_lexer(void) {
    lptr_t tmp = this_token;
    this_token = next_token;
    next_token = tmp;
    get_token(next_token);
}
