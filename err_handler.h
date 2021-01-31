/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * err_handler.h - This file contains the enums used for error handling, as
 * well as a couple of function definitions.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

/* This enum represents the various levels used by the logging system. */
typedef enum {
    LOG_INFO,       // print a message to the console
    LOG_WARNING,    // print a warning message and ignore the current input
    LOG_ERROR,      // print an error message and ignore the current input
    LOG_FATAL,      // print an error message and terminate the program
    LOG_OTHER = -1  // should not be used
} log_lev_t;


/* This enum represents the various types of errors.
 * The error type only affects what is printed to the console. */
typedef enum {
    ERR_LEX,        // lexical error
    ERR_SYNTAX,     // syntactic error
    ERR_TYPE,       // type reference error
    ERR_EVAL,       //evaluation error
    ERR_UNDEFINED,  // undefined variable error
    ERR_OTHER = -1  // should not be used; use logging to handle other errors
} err_type_t;


/* This function will log information to the console given a log_lev_t enum
 * and a log string. Use it for system level errors or debugging info. Output 
 * created by this function will not affect grading. */
extern int logging(log_lev_t, char*);

/* This function will display an error message based on the error type.
 * It has no effect when called multiple times on a single input.
 * 
 * For example, given the input: ((10 * true) + (10 / 0))
 * The program should print a type reference error and ignore the evaluation
 * error. */
extern int handle_error(err_type_t);
