/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * eval.c - This file contains the skeleton of functions to be implemented by
 * you. When completed, it will contain the code used to evaluate an expression
 * based on its AST.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"

extern bool is_binop(token_t);
extern bool is_unop(token_t);
char *strrev(char *str);

/* infer_type() - set the type of a non-root node based on the types of children
 * Parameter: A node pointer, possibly NULL.
 * Return value: None.
 * Side effect: The type field of the node is updated.
 * (STUDENT TODO)
 */

static void infer_type(node_t *nptr) {

    // 1) check if node pointer is null
    if (nptr == NULL) return;
     // 2) check the running status of the program
    if (terminate || ignore_input) return;
    
    if (nptr->node_type == NT_LEAF) return;
    
    
    infer_type(nptr->children[0]);
    infer_type(nptr->children[1]);
    nptr->type = nptr->children[0]->type;
    if (nptr->tok == TOK_EQ || nptr->tok == TOK_LT || nptr->tok == TOK_GT) {
        if (nptr->type == STRING_TYPE) {
            nptr->type = BOOL_TYPE;
        }
    }
    if (nptr->tok == TOK_QUESTION) {
        infer_type(nptr->children[2]);
    }


    return;

}

    // getting a core dump?


/* infer_root() - set the type of the root node based on the types of children
 * Parameter: A pointer to a root node, possibly NULL.
 * Return value: None.
 * Side effect: The type field of the node is updated. 
 */

static void infer_root(node_t *nptr) {
    if (nptr == NULL) return;
    // check running status
    if (terminate || ignore_input) return;

    if (nptr->node_type == NT_LEAF) return;

    // check for assignment
    if (nptr->type == ID_TYPE) {
        infer_type(nptr->children[1]);
    } else {
        for (int i = 0; i < 3; ++i) {
            infer_type(nptr->children[i]);
        }
        if (nptr->children[0] == NULL) {
            logging(LOG_ERROR, "failed to find child node");
            return;
        }
        nptr->type = nptr->children[0]->type;
    }
    return;
}

/* eval_node() - set the value of a non-root node based on the values of children
 * Parameter: A node pointer, possibly NULL.
 * Return value: None.
 * Side effect: The val field of the node is updated.
 * (STUDENT TODO) 
 */

static void eval_node(node_t *(nptr)) {
        if (nptr == NULL) return;
        
        if (terminate || ignore_input) return;

        if (nptr->node_type == NT_LEAF) return;

        for (int i = 0; i < 2; ++i) {
            eval_node(nptr->children[i]);
        }

            
        if (is_binop(nptr->tok)) {
            if (nptr->type == INT_TYPE) {
                if (nptr->tok == TOK_AND || nptr->tok == TOK_OR || nptr->children[1]->type != INT_TYPE) {
                     handle_error(ERR_TYPE);
                     return;
                }
            }
        }


        if (nptr->tok == TOK_PLUS) {

            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival + nptr->children[1]->val.ival;
            } else if (nptr->type == STRING_TYPE) {
                // 1) allocate memory, fill memory and copy strings over. 
                if (nptr->children[0]->type == STRING_TYPE && nptr->children[1]->type == STRING_TYPE) {
                    char *stringOne = malloc(strlen(nptr->children[0]->val.sval) + 1);
                    strcpy(stringOne, nptr->children[0]->val.sval);
                    char *stringTwo = malloc(strlen(nptr->children[1]->val.sval) + 1);
                    strcpy(stringTwo, nptr->children[1]->val.sval);
                    int requiredSpaceForCat = strlen(stringOne) + strlen(stringTwo) + 1;
                    char *combine = malloc(requiredSpaceForCat);
                    strcpy(combine, stringOne);
                    strcpy(combine + strlen(stringOne), stringTwo);
                    free(stringOne);
                    free(stringTwo);  
                    nptr->val.sval = combine;

                } else {
                    handle_error(ERR_TYPE);
                }
          
            } else if (nptr->type == BOOL_TYPE) {
                handle_error(ERR_TYPE);
            }
        } else if (nptr->tok == TOK_BMINUS) {
            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival - nptr->children[1]->val.ival;
            }
        } else if (nptr->tok == TOK_TIMES) {
           if (nptr->type == INT_TYPE) {
               nptr->val.ival = nptr->children[0]->val.ival * nptr->children[1]->val.ival;
        } else if (nptr->type == STRING_TYPE) {
            int factor = nptr->children[1]->val.ival;
            if (factor < 0) {
                handle_error(ERR_EVAL);
            }
            nptr->val.sval = calloc(1, strlen(nptr->children[0]->val.sval) * factor + 1);
            for (int i = 0; i < factor; ++i) {
                strcat(nptr->val.sval, nptr->children[0]->val.sval);
            }
           } else if (nptr->type == BOOL_TYPE) {
            
           }
        } else if (nptr->tok == TOK_DIV) {

            if (nptr->type == INT_TYPE) {
                if (nptr->children[1]->val.ival == 0) {
                    handle_error(ERR_EVAL);
                } else {
     
                    nptr->val.ival = nptr->children[0]->val.ival / nptr->children[1]->val.ival;
                }

            } else if (nptr->type == STRING_TYPE) {
                    handle_error(ERR_TYPE);

            } else if (nptr->type == BOOL_TYPE) {
                
            }
        } else if (nptr->tok == TOK_MOD) {
            if (nptr->children[0]->type != nptr->children[1]->type) {
                handle_error(ERR_TYPE);
            }
            if (nptr->type == INT_TYPE) {
                if (nptr->children[1]->val.ival == 0) {
                    handle_error(ERR_EVAL);
                } else {
                    nptr->val.ival = nptr->children[0]->val.ival % nptr->children[1]->val.ival;
                }
            }
        } else if (nptr->tok == TOK_AND) {
            if (nptr->type == INT_TYPE) {

            } else if (nptr->type == STRING_TYPE) {

            } else if (nptr->type == BOOL_TYPE) {
                nptr->val.bval = nptr->children[0]->val.bval && nptr->children[1]->val.bval;
            }
            
        } else if (nptr->tok == TOK_OR) {
            if (nptr->type == INT_TYPE) {
                
            } else if (nptr->type == STRING_TYPE) {

            } else if (nptr->type == BOOL_TYPE) {
                nptr->val.bval = nptr->children[0]->val.bval || nptr->children[1]->val.bval;
            }
        } else if (nptr->tok == TOK_LT) {
            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival < nptr->children[1]->val.ival;

            } else if (nptr->type == STRING_TYPE) {

            } else if (nptr->type == BOOL_TYPE) {

                 if (strcmp(nptr->children[0]->val.sval, nptr->children[1]->val.sval) < 0) {
                    nptr->val.bval = 1; 
                } else {
                    nptr->val.bval = 0;
                    }
                } 
   
        } else if (nptr->tok == TOK_GT) {
            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival > nptr->children[1]->val.ival;

            } else if (nptr->type == STRING_TYPE) {
   
            } else if (nptr->type == BOOL_TYPE) {
                if (nptr->children[0]->val.sval && nptr->children[1]->val.sval) {
                    if (strcmp(nptr->children[0]->val.sval, nptr->children[1]->val.sval) > 0) {
                        nptr->val.bval = 1; 
                    } else {
                        nptr->val.bval = 0;
                    }
                } else {
                    handle_error(ERR_TYPE); 
                }
            } 
        } else if (nptr->tok == TOK_EQ) {
            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival == nptr->children[1]->val.ival;
            } else if (nptr->type == STRING_TYPE) {
              


            } else if (nptr->type == BOOL_TYPE) {
                   if (strcmp(nptr->children[0]->val.sval, nptr->children[1]->val.sval) == 0) {
                    nptr->val.bval = 1; 
                } else {
                    nptr->val.bval = 0;
                }
            } 
        } else if (nptr->tok == TOK_UMINUS) {
            if (nptr->type == INT_TYPE) {
                nptr->val.ival = nptr->children[0]->val.ival * -1;
            } else if (nptr->type == STRING_TYPE) {
                nptr->val.sval = strrev(nptr->children[0]->val.sval);
            } else if (nptr->type == BOOL_TYPE) {
                handle_error(ERR_TYPE); 
            }
        } else if (nptr->tok == TOK_NOT) {
            if (nptr->type == INT_TYPE) {
                handle_error(ERR_TYPE);
            } 
        } else if (nptr->tok == TOK_QUESTION) {
            if (nptr->children[1]->type == INT_TYPE && nptr->children[2]->type == BOOL_TYPE) {
                handle_error(ERR_TYPE);
            } else {
                if (nptr->children[0]->val.bval == 1) {
                    eval_node(nptr->children[1]);
                    nptr->val.ival = nptr->children[1]->val.ival;
                } else {
                    eval_node(nptr->children[2]);
                    nptr->val.ival = nptr->children[2]->val.ival;
                }
            }
        }
        return;
}

/* eval_root() - set the value of the root node based on the values of children 
 * Parameter: A pointer to a root node, possibly NULL.
 * Return value: None.
 * Side effect: The val dield of the node is updated. 
 */

void eval_root(node_t *nptr) {
    if (nptr == NULL) return;
    // check running status
    if (terminate || ignore_input) return;

    // check for assignment
    if (nptr->type == ID_TYPE) {
        eval_node(nptr->children[1]);
        if (terminate || ignore_input) return;
        
        if (nptr->children[0] == NULL) {
            logging(LOG_ERROR, "failed to find child node");
            return;
        }
        put(nptr->children[0]->val.sval, nptr->children[1]);
        return;
    }

    for (int i = 0; i < 2; ++i) {
        eval_node(nptr->children[i]);
    }
    if (terminate || ignore_input) return;
    
    if (nptr->type == STRING_TYPE) {
        (nptr->val).sval = (char *) malloc(strlen(nptr->children[0]->val.sval) + 1);
        if (! nptr->val.sval) {
            logging(LOG_FATAL, "failed to allocate string");
            return;
        }
        strcpy(nptr->val.sval, nptr->children[0]->val.sval);
    } else {
        nptr->val.ival = nptr->children[0]->val.ival;
    }
    return;
}

/* infer_and_eval() - wrapper for calling infer() and eval() 
 * Parameter: A pointer to a root node.
 * Return value: none.
 * Side effect: The type and val fields of the node are updated. 
 */

void infer_and_eval(node_t *nptr) {
    infer_root(nptr);
    eval_root(nptr);
    return;
}

/* strrev() - helper function to reverse a given string 
 * Parameter: The string to reverse.
 * Return value: The reversed string. The input string is not modified.
 * (STUDENT TODO)
 */

char *strrev(char *str) {
    char *reversed = malloc(strlen(str) + 1);
    int revPos = 0;
    for (int i = strlen(str) - 1; i >= 0; i--) {
        reversed[revPos] = str[i];
        revPos++;
    }
    reversed[revPos] = '\0';
    return reversed;
}