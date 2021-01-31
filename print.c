/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * print.c - The printing module.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"

FILE *outfile = NULL;
FILE *errfile = NULL;
char *ci_prompt = NULL;

static char print_fmt;
static bool printing_bool;
static char fmt_string[100];
static char *lc_bool_print[] = {"false", "true"};
static char *uc_bool_print[] = {"FALSE", "TRUE"};

void format_and_print(node_t *nptr) {
    // check running status
    if (terminate) return;
    else if (ignore_input) {
        fprintf(outfile, "%s", ci_prompt);
        return;
    }
    
    if (!nptr) {
        logging(LOG_ERROR, "failed to print the node");
        fprintf(outfile, "%s", ci_prompt);
        return;
    }
    print_fmt = 'd';
    switch (nptr->type) {
        case INT_TYPE:
            if (nptr->children[1] && nptr->children[1]->type == FMT_TYPE)
                print_fmt = nptr->children[1]->val.fval;
            if (print_fmt == 'b' || print_fmt == 'B') {
                printing_bool = (nptr->val.ival != 0);
                sprintf(fmt_string, "\tans = %%s\n");
                fprintf(outfile, fmt_string, 
                        print_fmt == 'b' ? lc_bool_print[printing_bool] : uc_bool_print[printing_bool]);
                break;
            }
            sprintf(fmt_string, "\tans = %%0#%c\n", print_fmt);
            fprintf(outfile, fmt_string, nptr->val.ival);
            break;
        case BOOL_TYPE:
            if (nptr->children[1] && nptr->children[1]->type == FMT_TYPE) {
                print_fmt = nptr->children[1]->val.fval;
                if (print_fmt == 'b' || print_fmt == 'B') {
                    sprintf(fmt_string, "\tans = %%s\n");
                    fprintf(outfile, fmt_string, 
                            print_fmt == 'b' ? lc_bool_print[nptr->val.bval] : uc_bool_print[nptr->val.bval]);
                    break;
                }
            }
            sprintf(fmt_string, "\tans = %%0#%c\n", print_fmt);
            fprintf(outfile, fmt_string, nptr->val.bval);
            break;
        case STRING_TYPE:
            sprintf(fmt_string, "\tans = \"%%s\"\n");
            fprintf(outfile, fmt_string, nptr->val.sval);
            break;
        case ID_TYPE:
            format_and_print(nptr->children[1]);
            return;
        case FMT_TYPE:
        case NO_TYPE:
            logging(LOG_ERROR, "unsupported data type for printing");
            break;
    }
    fprintf(outfile, "%s", ci_prompt);
}
