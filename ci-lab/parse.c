/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * parse.c - This file contains the skeleton of functions to be implemented by
 * you. When completed, it will contain the code used to parse an expression and
 * create an AST.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include "ci.h"

/* Explained in ci.h */
extern lptr_t this_token, next_token;
extern void init_lexer(void);
extern void advance_lexer(void);

/* Valid format specifers */
static const char *VALID_FMTS = "dxXbB";

/* The only reserved identifiers are "true" and "false" */
static const int NUM_RESERVED_IDS = 2;
static const struct {
    char *id;
    token_t t;
} reserved_ids[] = {
    {"true", TOK_TRUE},
    {"false", TOK_FALSE}
};

/* is_binop() - return true if a token represents a binary operator
 * Parameter: Any token
 * Return value: true if t is a binary operator, false otherwise */
bool is_binop(token_t t) {
    return t >= TOK_PLUS && t <= TOK_EQ;
}

/* is_unop() - return true if a token represents a unary operator
 * Parameter: Any token
 * Return value: true if t is a unary operator, false otherwise */
bool is_unop(token_t t) {
    return t >= TOK_UMINUS && t <= TOK_NOT;
}

/* id_is_fmt_spec() - return true if a string is a format specifier
 * Parameter: Any string
 * Return value: true if s is format specifier, false otherwise */
bool id_is_fmt_spec(char *s) {
    return strlen(s) == 1 && strspn(s, VALID_FMTS) == 1;
}

/* check_reserved_ids() - check if a given string is a reserved identifier
 * Parameter: Any string
 * Return value: true if s is a reserved identifier, false otherwise */
static token_t check_reserved_ids(char *s) {
    for (int i = 0; i < NUM_RESERVED_IDS; i++)
        if (strcmp(reserved_ids[i].id, s) == 0) return reserved_ids[i].t;
    return TOK_INVALID;
}

/* build_leaf() - create a leaf node based on this_token and / or next_token
 * Parameter: none
 * Return value: pointer to a leaf node
 * (STUDENT TODO) */
static node_t *build_leaf(void) {

    // 1) Allocate memory 2) Fill that memory

    // allocate memory for the leaf node
    node_t *leaf = calloc(1, sizeof(node_t));

    if (! leaf) {
        // handle memory allocation failure
        logging(LOG_ERROR, "failed to allocate node");
        return NULL;
    }

    // set the node struct's fields pt.1
    // we are filling the memory here
    leaf->node_type = NT_LEAF;

    // set the node struct's fields pt.2 (this is based on the type of token).
    if (this_token->ttype == TOK_NUM) {
        leaf->type = INT_TYPE;
        leaf->val.ival = atoi(this_token->repr);
    } else if (this_token->ttype == TOK_TRUE || this_token->ttype == TOK_FALSE) {
        leaf->type = BOOL_TYPE;
        if (this_token->ttype == TOK_TRUE) {
            leaf->val.bval = 1;
        } else {
            leaf->val.bval = 0;
        }
    } else if (this_token->ttype == TOK_STR) {
        leaf->type = STRING_TYPE;
        leaf->val.sval = malloc(strlen(this_token->repr) + 1);
        strcpy(leaf->val.sval, this_token->repr);
        // leaf->val.sval = this_token->repr;
    } else if (this_token->ttype == TOK_FMT_SPEC) {
        leaf->type = FMT_TYPE;
        // hmm, what goes here?
        leaf->val.fval = this_token->repr[0];
    } else if (this_token->ttype == TOK_ID && next_token->ttype == TOK_ASSIGN) {
        leaf->type = ID_TYPE;
        // char value!
        leaf->val.sval = malloc(strlen(this_token->repr) + 1);
        strcpy(leaf->val.sval, this_token->repr);
    } else if (this_token->ttype == TOK_ID && next_token->ttype != TOK_ASSIGN) {
        entry_t *temp = malloc(sizeof(entry_t) + 1);
        temp = get(this_token->repr);
        // doesnt exist! 
        if (temp == NULL) {
            leaf->type = NO_TYPE;
        } else {
            // set the type and value!
            leaf->type = temp->type;
            // lets update based on type. that makes sense...
            if (leaf->type == STRING_TYPE) {
                leaf->val.sval = malloc(strlen(temp->val.sval));
                strcpy(leaf->val.sval, temp->val.sval);
            } else if (leaf->type == INT_TYPE) {
                leaf->val.ival = temp->val.ival;
            } else {
                // not string or int? its gotta be a bool!
                leaf->val.bval = temp->val.bval;
            }
        }
    }


    return leaf;
}

/* build_exp() - parse an expression based on this_token and / or next_token
 * Make calls to build_leaf() or build_exp() if necessary. 
 * Parameter: none
 * Return value: pointer to an internal node
 * (STUDENT TODO */
static node_t *build_exp(void) {
    // check running status
    if (terminate || ignore_input) return NULL;
    
    token_t t;

    // The case of a leaf node is handled for you
    if (this_token->ttype == TOK_NUM)
        return build_leaf();
    if (this_token->ttype == TOK_STR)
        return build_leaf();
    // handle the reserved identifiers, namely true and false
    if (this_token->ttype == TOK_ID) {
        if ((t = check_reserved_ids(this_token->repr)) != TOK_INVALID) {
            this_token->ttype = t;
        }
        return build_leaf();
    } else {
        // (STUDENT TODO) implement the logic for internal nodes.
        // Overall idea: create a node when you hit ( and build an expression until you hit )

        // aha! We have gotten to the else statement. 
        // We know that we are going to encounter a left parenthesis.

        // 1) construct the internal node
        node_t *internalNode = calloc(1, sizeof(node_t));

        if (! internalNode) {
            // handle memory allocation failure
            logging(LOG_ERROR, "failed to allocate node");
            return NULL;
        }

        if (this_token->ttype != TOK_LPAREN) {
            handle_error(ERR_SYNTAX);
        }
        
        // 2) move forward in the stream.
        advance_lexer();
        if (this_token->ttype == TOK_QUESTION || is_binop(this_token->ttype) || (is_unop(this_token->ttype) && next_token->ttype == TOK_RPAREN)) {
            handle_error(ERR_SYNTAX);
        }
        // check if it is a leaf. 
        if (next_token->ttype == TOK_RPAREN) {
            internalNode = build_leaf();
        } else {
            // we know it is not a leaf.
            internalNode->node_type = NT_INTERNAL;

            if (is_unop(this_token->ttype)) {
                internalNode->tok = this_token->ttype;
                advance_lexer();
                if (this_token->ttype == TOK_RPAREN) {
                    handle_error(ERR_SYNTAX);

                }
                internalNode->children[0] = build_exp(); 
                advance_lexer();
                return internalNode;
            } else {
                internalNode->children[0] = build_exp(); 
                bool isQuestion = next_token->ttype == TOK_QUESTION;
                if (!is_binop(next_token->ttype) && !isQuestion) {
                    handle_error(ERR_SYNTAX);
                }
                advance_lexer();
                internalNode->tok = this_token->ttype;
                if (is_binop(next_token->ttype) || next_token->ttype == TOK_RPAREN) {
                    handle_error(ERR_SYNTAX);
                }

                
                advance_lexer();
                // 3) set right child to result of build exp.
                internalNode->children[1] = build_exp();
                if (isQuestion) {
                    advance_lexer();
                    if (this_token->ttype != TOK_COLON) {
                        handle_error(ERR_SYNTAX);
                    }
                    advance_lexer();
                    internalNode->children[2] = build_exp();
                } 
            }
        }
        if (next_token->ttype != TOK_RPAREN) {
            handle_error(ERR_SYNTAX);
        }
        advance_lexer();
        return internalNode;
        }
    }

/* build_root() - construct the root of the AST for the current input
 * This function is provided to you. Use it as a reference for your code
 * Parameter: none
 * Return value: the root of the AST */
static node_t *build_root(void) {
    // check running status
    if (terminate || ignore_input) return NULL;

    // allocate memory for the root node
    node_t *ret = calloc(1, sizeof(node_t));
    if (! ret) {
        // calloc returns NULL if memory allocation fails
        logging(LOG_FATAL, "failed to allocate node");
        return NULL;
    }

    // set the node struct's fields
    ret->node_type = NT_ROOT;
    ret->type = NO_TYPE;

    // (EEL-2) check for variable assignment
    if (this_token->ttype == TOK_ID && next_token->ttype == TOK_ASSIGN) {
        if (check_reserved_ids(this_token->repr) != TOK_INVALID) {
            logging(LOG_ERROR, "variable name is reserved");
            return ret;
        }
        ret->type = ID_TYPE;
        ret->children[0] = build_leaf();
        advance_lexer();
        advance_lexer();
        ret->children[1] = build_exp();
        if (next_token->ttype != TOK_EOL) {
            handle_error(ERR_SYNTAX);
        }
        return ret;
    }
    
    // build an expression based on the current token
    // this will be where the majority of the tree is recursively constructed
    ret->children[0] = build_exp();

    // if the next token is End of Line, we're done
    if (next_token->ttype == TOK_EOL)
        return ret;
    else {                                     
    /* At this point, we've finished building the main expression. The only
     * syntactically valid tokens that could remain would be format specifiers */    
        
        // check that our next token is a format specifier
        if (next_token->ttype != TOK_SEP) {
            handle_error(ERR_SYNTAX);
            return ret;
        }

        advance_lexer();
        
        // check that there is an ID following the format specifier
        if (next_token->ttype != TOK_ID) {
            handle_error(ERR_SYNTAX);
            return ret;
        }

        // check that the ID is a format specifier ID
        if (id_is_fmt_spec(next_token->repr))
            next_token->ttype = TOK_FMT_SPEC;
        if (next_token->ttype != TOK_FMT_SPEC) {
            handle_error(ERR_SYNTAX);
            return ret;
        }

        advance_lexer();

        // build the leaf for the format specifier. 
        // if any tokens besides EOL remain, the syntax is not valid
        ret->children[1] = build_leaf();
        if (next_token->ttype != TOK_EOL) {
            handle_error(ERR_SYNTAX);
            return ret;
        }
        return ret;
    }

    // this return statement will only be reached if there was a syntax error
    handle_error(ERR_SYNTAX);
    return ret;
}

/* read_and_parse - return the root of an AST representing the current input
 * Parameter: none
 * Return value: the root of the AST */
node_t *read_and_parse(void) {
    init_lexer(); // given to us.
    return build_root(); // calls build_exp()
}

/* cleanup() - given the root of an AST, free all associated memory
 * Parameter: The root of an AST
 * Return value: none
 * (STUDENT TODO) */
void cleanup(node_t *nptr) {

    // we have reached a leaf. 
    if (nptr == NULL) {
        return;
    }
    
    if (nptr->type == STRING_TYPE) {
        free(nptr->val.sval);
    } 
    // if (nptr->type == ID_TYPE) {
        // free(nptr->id);
    // }
    // joyce recommendation-> handle ID freeing as well.

    // recurse to the left
    cleanup(nptr->children[0]);

    cleanup(nptr->children[1]);

    cleanup(nptr->children[2]);

    // free memory for the current node!
    free(nptr);

    return;
}