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

#define MAX_PRINT_DEPTH 100
int indents[MAX_PRINT_DEPTH];

void print_tree_helper(node_t* node, int level) {
    if (!node)
        return;
    if (level >= MAX_PRINT_DEPTH) {
        printf("EXCEDED MAX PRINT DEPTH OF %d. Consider changing in print.c\n", MAX_PRINT_DEPTH);
        indents[level-1]--;
        return;
    }
    for(int i = 0; i < level; i++) {
        if (indents[i])
            printf("|   ");
        else
            printf("    ");
    }
    printf("\n");
    for(int i = 0; i < level; i++) {
        if (i >= level - 1) {
            if (indents[i])
                printf("|---");
            else
                printf("----");
        } else {
            if (indents[i])
                printf("|   ");
            else
                printf("    ");
        }
    }
    indents[level-1]--;
    
    int count = 0;
    for(int i = 0; i < 3; i++) {
        if (node->children[i])
            count++;
        else
            break;
    }
    indents[level] = count;

    if (node->node_type == NT_ROOT) {
        printf("ROOT");
    } else {
        switch (node->tok) {
            case TOK_ID:
                if (node->type == ID_TYPE)
                    printf("id: %s", node->val.sval);
                else if(node->type == INT_TYPE)
                    printf("%d", node->val.ival);
                else if(node->type == BOOL_TYPE && node->val.bval)
                    printf("true");
                else if(node->type == BOOL_TYPE && !node->val.bval)
                    printf("false");
                else if (node->type == STRING_TYPE)
                    printf("\"%s\"", node->val.sval);
                else
                    printf("Invalide node type: %d", node->type);
                break;
            case TOK_NUM:
                printf("%d", node->val.ival);
                break;
            case TOK_TRUE:
                printf("true");
                break;
            case TOK_FALSE:
                printf("false");
                break;
            case TOK_STR:
                printf("\"%s\"", node->val.sval);
                break;
            case TOK_QUESTION:
                printf("?");
                break;
            case TOK_COLON:
                printf(":");
                break;
            case TOK_PLUS:
                printf("+");
                break;
            case TOK_BMINUS:
                printf("(-)");
                break;
            case TOK_TIMES:
                printf("*");
                break;
            case TOK_DIV:
                printf("/");
                break;
            case TOK_MOD:
                printf("%%");
                break;
            case TOK_AND:
                printf("&");
                break;
            case TOK_OR:
                printf("(|)");
                break;
            case TOK_LT:
                printf("<");
                break;
            case TOK_GT:
                printf(">");
                break;
            case TOK_EQ:
                printf("~");
                break;
            case TOK_UMINUS:
                printf("_");
                break;
            case TOK_NOT:
                printf("!");
                break;
            case TOK_ASSIGN:
                printf("=");
                break;
            case TOK_IDENTITY:
                printf("()");
                break;
            case TOK_FMT_SPEC:
                printf("# %c", node->val.fval);
                break;
            default:
                printf("Invalid node token: %d", node->tok);
        }
    }
    printf("\n");

    for(int i = 0; i < 3 && indents[level] > 0; i++) {
        print_tree_helper(node->children[i], level + 1);
    }
}

void print_tree(node_t* node) {
    memset(&indents, 0, sizeof(int) * MAX_PRINT_DEPTH);

    print_tree_helper(node, 0);
}
