/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * ci.h - A header file with the functions and variables that will be used by
 * ci in order to run. ci.h contains many important functions and variables.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

/* #include statements
 * The following #include lines will allow ci.h to access the functions and
 * variables found in these files. This is similar to importing packages and
 * libraries in Java or Python.
 * 
 * #include <...> will generally search for standard library files, like 
 * stdio.h. #include "..." will generally search for local files, like value.h. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "token.h"
#include "value.h"
#include "type.h"
#include "node.h"
#include "err_handler.h"
#include "variable.h"

/* Function declarations
 * The following function declarations allow any file that #includes ci.h
 * to use these functions. The extern keyword means they are implemented in
 * a file other than ci.c/h. */

/* Provided function to process commandline arguments given to ci. */
extern void handle_args(int, char **);

/* Provided function to initialize the ci interface. */
extern void init(void);

/* Provided function to print the final statements of the ci interface. */
extern void finalize(void);

/* Provided function to format & print the answer of an expression. */
extern void format_and_print(node_t *);

/* Provided function to print the (sub)tree from a given node. */
extern void print_tree(node_t *);

/* Provided function to initialize the lexer. The lexer will process the user
 * input and provide individual operators/operands as "tokens". These tokens 
 * will be accessible from the this_token and next_token variables. */
extern void init_lexer(void);

/* Provided function to utilize the lexer to update this_token and 
 * next_token. */
extern void advance_lexer(void);

/* (STUDENT TODO)
 * This function will use the provided lexer & the student's parse tree
 * implementation to parse input to the ci. You won't modify this specific
 * function, but rather the functions which it calls. */
extern node_t *read_and_parse(void);

/* (STUDENT TODO)
 * This function will infer the types of operands & evaluate the parsed input.
 * You won't modify this specific function, but rather the functions which it
 * calls. */
extern void infer_and_eval(node_t *);

/* (STUDENT TODO)
 * This function will free the memory allocated for a given parse tree. */
extern void cleanup(node_t *);

/* (EEL-2) These functions will perform variable insertion or searching in a
 * hashtable. You won't touch these until finishing EEL-1. */
void put(char *id, node_t *nptr);
extern entry_t* get(char* id);

/* Variable declarations
 * The following variable declarations allow any file that #includes ci.h to
 * access these variables. The extern keyword means they are provided by a file
 * other than ci.c or ci.h. */

/* The infile, outfile, and errfile variables are all used to tell the program 
 * where to obtain input, where to place output, and where to log errors. They
 * are similar to Java's System.in, System.out, and System.err. */
extern FILE *infile;
extern FILE *outfile;
extern FILE *errfile;

/* This is a string containing the prompt that will be displayed by the ci. */
extern char *ci_prompt;

/* These variables are pointers to "lexeme" structs, defined in token.h. They
 * are updated by calls to init_lexer and advance_lexer. More information about
 * the lexeme struct can be found in token.h. */
extern lptr_t this_token, next_token;

/* These are booleans used to control program execution.
 * If ignore_input is true, the current input will no longer be processed. 
 * If terminate is true, the ci program will terminate. */
extern bool terminate, ignore_input;

/* (EEL-2) The hashtable storing all defined variables. */
extern table_t *var_table;
