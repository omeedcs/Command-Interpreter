/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * node.h - This file contains the declaration of the node struct, as well as
 * the enum used to classify the type of a node.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

/* This enum is used to classify nodes by their positions in the tree. */
typedef enum {
    NT_LEAF,
    NT_INTERNAL,
    NT_ROOT
} node_type_t;

/* The node struct. By using typedef, we create the shorthands node_t and nptr_t
 * for a variable's type. */
typedef struct node {
    token_t tok;                // represented input token
    node_type_t node_type;      // node type
    value_t val;                // data value defined in value.h
    type_t type;                // data type defined in type.h
    struct node *children[3];   // array of children nodes (3 is the maximum number of children)
} node_t, *nptr_t;
