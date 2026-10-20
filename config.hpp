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
    ADD = 9,
    SUB = 10,
    MUL = 11,
    DIV = 12,
    POW = 13,
    JMP = 14,
    JA = 15,
    JAE = 16,
    JB = 17,
    JBE = 18,
    JE = 19,
    JNE = 20,
    JSUN = 21,
    DRAW = 22,
    CALL = 23,
    RET = 24
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