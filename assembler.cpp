#include <stdio.h>
#include <string.h>

#include "C:\Users\3DFAB\Documents\GitHub\StringSorter\sorting.h"

enum commands{
    ERR = -1,
    HLT = 0,
    OUT = 1,
    PUSH = 2,
    POP = 3,
    PUSHR = 4,
    POPR = 5,
    ADD = 6,
    SUB = 7,
    MUL = 8,
    DIV = 9
};

enum registers{
    RERR = 0,
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

struct assembler{
    int *byteCode;
    unsigned int byteCodeLen;
};

struct instruction{
    commands cmd;
    int arg;
};

void Translator(Info *programInfo, assembler *ass);
commands CheckCommandNoArgs(char *str);
commands CheckCommandWithArgs(char *str);
int CheckArgument(char *str, instruction *instr);
registers IsRegister(char *str);
void CreateCodeFile(Info *programInfo, assembler *ass);

int main(){
    Info *env = {};

    assembler *assembler;

    env->fileName = "code.asm"; //TODO - argv

    OpenInFile(env);

    ReadInFile(env);

    ChangeEndlsToNulls(env);

    BufferToLinesFragmentation(env);

    Translator(env, assembler);

    CreateCodeFile(env, assembler);

    return 0;
}

void Translator(Info *programInfo, assembler *ass){
    
    char *tempStr1 = (char *)calloc(5, sizeof(char));
    char *tempStr2 = (char *)calloc(sizeof(int) * 8, sizeof(char));

    ass->byteCodeLen = 2 * programInfo->arrSize;

    ass->byteCode = (int *)calloc(ass->byteCodeLen, sizeof(int));

    instruction instr;

    for(int index = 0; index < programInfo->arrSize; index++){
        int check = sscanf((programInfo->arrLines)[index].line, "%s %s", tempStr1, tempStr2);

        if (check == 1){
            instr.cmd = CheckCommandNoArgs(tempStr1);

            if (instr.cmd == ERR){
                printf("Command error\n");
                abort();
            }

            ass->byteCode[index] = (int)instr.cmd;

            continue;
        }
        
        if (check == 2){
            instr.cmd = CheckCommandWithArgs(tempStr1);
            instr.arg = CheckArgument(tempStr2, &instr);
            if (instr.cmd == ERR){
                printf("Command error\n");
                abort();
            }
            if (instr.arg == RERR){
                printf("ArgumentError");
                abort();
            }

            ass->byteCode[index] = instr.cmd;
            ass->byteCode[index + 1] = instr.arg;
            index++; 
        }
        

        else{
            instr.cmd = ERR;
        }
    }
    free(tempStr1);
    free(tempStr2);
}


commands CheckCommandNoArgs(char *str){
    if(strcmp(str, "HLT") == 0){
        return HLT;
    }
    if(strcmp(str, "OUT") == 0){
        return OUT;
    }
    if(strcmp(str, "ADD") == 0){
        return ADD;
    }
    if(strcmp(str, "SUB") == 0){
        return SUB;
    }
    if(strcmp(str, "MUL") == 0){
        return MUL;
    }
    if(strcmp(str, "DIV") == 0){
        return DIV;
    }
    else{
        return ERR;
    }
}

commands CheckCommandWithArgs(char *str){
    if(strcmp(str, "PUSH") == 0){
        return PUSH;
    }
    if(strcmp(str, "POP") == 0){
        return POP;
    }
    else{
        return ERR;
    }
}

int CheckArgument(char *str, instruction *instr){
    int tempInt = 0;
    int check = sscanf(str, "%d", &tempInt);
    if (check == 1){
        return tempInt;
    }
    else{
        char tempStr[3];
        sscanf(str, "%s", tempStr);
        registers tempReg = IsRegister(tempStr);

        if (instr->cmd == PUSH){            
            if(tempReg){
                instr->arg = (int)tempReg;
                instr->cmd = PUSHR;
            }
            else{
                instr->cmd = ERR;
            }
        }
        if (instr->cmd == POP){            
            if(tempReg){
                instr->arg = (int)tempReg;
                instr->cmd = POPR;
            }
            else{
                instr->cmd = ERR;
            }
        }
    }
    return RERR;
}

registers IsRegister(char *str){
    if(strcmp(str, "AX") == 0){
        return RAX;
    }
    if(strcmp(str, "BX") == 0){
        return RBX;
    }
    if(strcmp(str, "CX") == 0){
        return RCX;
    }
    if(strcmp(str, "DX") == 0){
        return RDX;
    }
    else{
        return RERR;
    }
}
void CreateCodeFile(Info *programInfo, assembler *ass){

    programInfo->fileOut = fopen("code.txt", "w");
    for(int i = 0; i < ass->byteCodeLen; i++){
        fprintf(programInfo->fileOut, "%d", ass->byteCode[i]);
    }
    
    free(ass->byteCode);
    fclose(programInfo->fileOut);
}