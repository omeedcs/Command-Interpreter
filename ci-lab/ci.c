/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * ci.c - The top-level loop of the interpreter.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"

int main(int argc, char* argv[]) {
    handle_args(argc, argv);
    init();
    while (! terminate) {
        ignore_input = false;
        node_t *nptr = read_and_parse(); // located in parse.c
        infer_and_eval(nptr); // located in eval.c
        format_and_print(nptr);
        cleanup(nptr);
    }
    finalize();
    return EXIT_SUCCESS;
}
