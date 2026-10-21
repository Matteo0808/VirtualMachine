#ifndef CONFIG_H
#define CONFIG_H



enum commands{
    ERR = -1,
    HLT = 0,
    IN = 1,
    OUT = 2,
    PUSH = 3,
    POP = 4,
    PUSHR = 5,
    POPR = 6,
    PUSHM = 7,
    POPM = 8,
    PUSHRM = 9,
    POPRM = 10,    
    ADD = 11,
    SUB = 12,
    MUL = 13,
    DIV = 14,
    POW = 15,
    JMP = 16,
    JA = 17,
    JAE = 18,
    JB = 19,
    JBE = 20,
    JE = 21,
    JNE = 22,
    JSUN = 23,
    DRAW = 24,
    CALL = 25,
    RET = 26,
    FACT = 27,
    FIB = 28
};

enum registers{
    RERR = -1,
    RSX = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};



#endif