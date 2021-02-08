/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * value.h - This file contains the definition of the value type, which is a
 * union of all possible values a node may have.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

/* Union type in which the result of eval()ing an EEL expression is stored. 
 * The value is arbitrary if the type of the node is NO_TYPE.
 * 
 * A union type means all of these fields share memory, so updating one will
 * affect the others. You should use the token's type to determine which field 
 * to access. */
typedef union value {
    int ival;           // value if type is INT_TYPE
    bool bval;          // value if type is BOOL_TYPE
    char fval;          // value if type is FORMAT_TYPE
    char *sval;         // value if type is STRING_TYPE
} value_t, *vptr_t;
