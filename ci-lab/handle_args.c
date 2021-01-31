/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * handle_args.c - Module for handling command-line arguments.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"

static char printbuf[100];

void handle_args(int argc, char **argv) {
    int option;
    outfile = stdout;
    errfile = stderr;

    while ((option = getopt(argc, argv, "i:o:")) != -1) {
        switch(option) {
            case 'i':
                if ((infile = fopen(optarg, "r")) == NULL) {
                    sprintf(printbuf, "input file %s not found", optarg);
                    logging(LOG_FATAL, printbuf);
                    return;
                }
                break;
            case 'o':
                if ((outfile = fopen(optarg, "w")) == NULL) {
                    sprintf(printbuf, "failed to open output file %s", optarg);
                    logging(LOG_FATAL, printbuf);
                    return;
                }
                break;
            default:
                sprintf(printbuf, "Ignoring unknown option %c", optopt);
                logging(LOG_INFO, printbuf);
                break;
        }
    }
    for(; optind < argc; optind++) { // when some extra arguments are passed
        sprintf(printbuf, "Ignoring extra argument %s", argv[optind]);
        logging(LOG_INFO, printbuf);
    }
    if (infile == NULL) infile = stdin;
    return;
}
