enum commands{
    ERR = -1,
    HLT = 0,
    OUT = 1,
    PUSH = 2,
    POP = 3,
    PUSHR = 4,
    POPR = 5,
    PUSHM = 6,
    POPM = 7,
    ADD = 8,
    SUB = 9,
    MUL = 10,
    DIV = 11,
    POW = 12,
    JMP = 13,
    JA = 14,
    JAE = 15,
    JB = 16,
    JBE = 17,
    JE = 18,
    DRAW = 19
};

enum registers{
    RERR = -1,
    RSX = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};
