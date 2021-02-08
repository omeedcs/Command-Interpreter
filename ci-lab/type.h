/**************************************************************************
 * C S 429 EEL interpreter
 * 
 * type.h - This file contains the definition of type_t, used to classify the
 * type of an EEL expression.
 * 
 * Copyright (c) 2021. S. Chatterjee, X. Shen, T. Byrd. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

/* Enumerated type giving the possible EEL value types. The value is arbitrary
 * if the type of the node is NO_TYPE. */
typedef enum {
    INT_TYPE,
    BOOL_TYPE,
    STRING_TYPE,
    FMT_TYPE,
    ID_TYPE,
    NO_TYPE = -1
} type_t;
