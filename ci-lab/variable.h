/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * err_handler.c - The error-handling module.
 * variable.h - Hash table implementation to store defined variables (EEL-2).
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 


#define CAPACITY 100

/* Entry of the hashtable, which stores a defined variable.
 * A next pointer is provided to handle collision with linked list */
typedef struct entry {
    char *id;               // variable name used for indexing
    value_t val;            // variable value
    type_t type;            // variable data type
    struct entry *next;     // points for linked list implementation
} entry_t;

/* Hashtable that stores all the defined variables. */
typedef struct table {
    entry_t **entries;
} table_t;

/* Initialize the global hashtable. */
extern void init_table(void);

/* Release allocated memory for the table. */
extern void delete_table(void);

/* list all entries stored in the table. */
extern void print_table(void);
