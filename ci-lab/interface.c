/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * interface.c - Initialization and finalization routines for the interpreter.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"
#include "ansicolors.h"

char default_ci_prompt[] = ANSI_BOLD ANSI_COLOR_BLUE "UTCS429-S2021-ci>>> " ANSI_RESET;
const char ci_logo[] = ANSI_COLOR_MAGENTA "`·.¸¸.·´¯`·.¸><(((º>" ANSI_RESET;
const char author[] = ANSI_BOLD ANSI_COLOR_RED "Omeed Tehrani - oht83" ANSI_RESET;

static void print_init_msg(void) {
    time_t t;
    
    fprintf(outfile, "Welcome to the EEL interpreter\n\n");
    fprintf(outfile, "%s\n\n", ci_logo);
    fprintf(outfile, "Author: %s\n", author);
    assert(time(&t) != -1);
    fprintf(outfile, "Run begun at %s\n\n", ctime(&t));
}

void init(void) {
    if (! ci_prompt) ci_prompt = default_ci_prompt;
    init_table();
    if (outfile != stdout) {
        ci_prompt = "";
        return;
    }
    print_init_msg();
    fprintf(outfile, "%s", ci_prompt);
    return;
}

void finalize(void) {
    if (outfile != stdout) return;
    time_t t;
    assert(time(&t) != -1);
    delete_table();
    fprintf(outfile, "Run ended at %s\n", ctime(&t));
    fprintf(outfile, ANSI_BOLD "Goodbye!\n\n" ANSI_RESET);
    return;
}
