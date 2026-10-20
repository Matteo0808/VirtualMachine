#include <stdio.h>
#include <string.h>
#include "strhandler.hpp"
#include "stack.hpp"
#include "config.hpp"

const int labelSize = 16;
const char *fileOutName = "code.txt";

enum args{
    NO_ARGS = 1,
    ONE_ARG = 2
};

struct assembler{
    int *byteCode;
    unsigned int byteCodeLen;
    int instrPtr;
    int Label[labelSize];
};

struct instruction{
    commands cmd;
    int arg;
};

void AssemblerConstructor(strInfo **programInfo, assembler **ass, const char *fileName);
void AssemblerDestructor(strInfo *programInfo, assembler *ass);

void DoubleAssembling(strInfo *programInfo, assembler *ass);
void Translator(strInfo *programInfo, assembler *ass);

commands CheckCommandNoArgs(char *str);
commands CheckCommandWithArgs(char *str);
int CheckArgument(char *str, instruction *instr);
registers IsRegister(char *str);
commands CheckCommandJump(char *str);
commands CheckCommandMemory(char *str);

void SetLabelMinusOne(assembler *ass);

void CreateCodeFile(strInfo *programInfo, assembler *ass);

//TODO - jmp??? ask

//TODO - comments

//TODO - grafic

//TODO - how to remake asm file

//TODO - how to fill file? make c program?

int main(int argc, char *argv[]){

    if(argc != 2){
        abort();
    }
    
    strInfo *env;
    
    assembler *assembler;

    AssemblerConstructor(&env, &assembler, argv[1]); //TODO - argv

    OpenInFile(env);

    ReadInFile(env);

    OutputUnsortedText(env);

    ChangeEndlsToNulls(env);

    BufferToLinesFragmentation(env);

DEBUG(
    printf("arrLinesSize = %d\n",env->arrSize);

    for (int i = 0; i < env->arrSize; i++){
        printf("line %d = <<%s>>\n", i, env->arrLines[i].line);
    }
)

    DoubleAssembling(env, assembler);

    //Translator(env, assembler);

    printf("Labels:\n");
    for(int i = 0; i < 16; i++){
        printf("label %d: %d\n", i, assembler->Label[i]);
    }


    for (int i = 0; i < assembler->byteCodeLen; i++){
        printf("%d ", assembler->byteCode[i]);
    }


    CreateCodeFile(env, assembler);

    AssemblerDestructor(env, assembler);

    return 0;
}

void AssemblerConstructor(strInfo **programInfo, assembler **ass, const char *fileName){

    *programInfo = (strInfo *)calloc(1, sizeof(strInfo));

    *ass = (assembler *)calloc(1, sizeof(assembler));

    SetLabelMinusOne(*ass);

    (*ass)->instrPtr = 0;

    (*programInfo)->fileName = fileName;
}

void Translator(strInfo *programInfo, assembler *ass){

    ass->byteCodeLen = 2 * programInfo->arrSize;

    ass->byteCode = (int *)calloc(ass->byteCodeLen, sizeof(int));

    instruction instr = {ERR, RERR};

    for (int index = 0; index < programInfo->arrSize; index++){
                
        char strCommand[5];
        char strArgument[11];
        int intArgument = 0;

        int checkSscanf = sscanf((programInfo->arrLines)[index].line, "%s %s", strCommand, strArgument);
        
        if (checkSscanf == NO_ARGS){

            printf("check = 1; cmd = %s\n", strCommand);

            int intLabel = 0;

            if (sscanf(strCommand, ":%d", &intLabel) == 1){

                ass->Label[intLabel] = ass->instrPtr;

                printf("label = %d, instr = %d\n", intLabel, ass->instrPtr);

                continue;
            }
            

            instr.cmd = CheckCommandNoArgs(strCommand);

            if (instr.cmd == ERR){
                printf("Command error\n");
                abort();
            }
            
            ass->byteCode[ass->instrPtr] = (int)instr.cmd;

            ass->instrPtr++;

            continue;
        }
        
        if (checkSscanf == ONE_ARG){

            if(sscanf((programInfo->arrLines)[index].line, "%s [%d]", &strCommand, &intArgument) == 2){

                printf("memory?\n");

                ass->byteCode[ass->instrPtr] = CheckCommandMemory(strCommand);
                
                ass->byteCode[ass->instrPtr + 1] = intArgument;

                ass->instrPtr += 2;

                continue;
            }

            if(sscanf((programInfo->arrLines)[index].line, "%s :%d", &strCommand, &intArgument) == 2){

                printf("jump?\n");
                
                ass->byteCode[ass->instrPtr] = CheckCommandJump(strCommand);

                ass->byteCode[ass->instrPtr + 1] = ass->Label[intArgument];

                ass->instrPtr += 2;

                continue;
            }

            printf("check = 2; cmd = %s\n", strCommand);
            printf("check = 2; arg = %s\n", strArgument);

            instr.cmd = CheckCommandWithArgs(strCommand);

            instr.arg = CheckArgument(strArgument, &instr);

            if (instr.cmd == ERR){
                printf("Command error\n");
                abort();
            }
            if (instr.arg == RERR){
                printf("ArgumentError");
                abort();
            }

        DEBUG(
            printf("\nBIGPRINTF:\nass->instrPtr = %d\ninstr.cmd = %d\ninstr.arg = %d\n\n", ass->instrPtr, instr.cmd, instr.arg);
        )

            ass->byteCode[ass->instrPtr] = instr.cmd;
            ass->byteCode[ass->instrPtr + 1] = instr.arg;

        //DEBUG(
            printf("command = <<%d>>, argument = <<%d>>\n", ass->byteCode[ass->instrPtr], ass->byteCode[ass->instrPtr + 1]);
        //)

            ass->instrPtr += 2;
        }        
        else{
            instr.cmd = ERR;
        }
    //DEBUG( 
        printf("ass->byteCode:\n");
        for(int i = 0; i < ass->byteCodeLen; i++){
            printf("%d: [%d]\n", i, ass->byteCode[i]);
        }
    //)
    }
}

void DoubleAssembling(strInfo *programInfo, assembler *ass){
    Translator(programInfo, ass);

    ass->instrPtr = 0;

    Translator(programInfo, ass);
}


commands CheckCommandNoArgs(char *str){

    if (strcmp(str, "HLT") == 0){
        return HLT;
    }

    if(strcmp(str, "IN") == 0){
        return IN;
    }

    if (strcmp(str, "OUT") == 0){
        return OUT;
    }

    if (strcmp(str, "ADD") == 0){
        return ADD;
    }

    if (strcmp(str, "SUB") == 0){
        return SUB;
    }

    if (strcmp(str, "MUL") == 0){
        return MUL;
    }

    if (strcmp(str, "DIV") == 0){
        return DIV;
    }

    if (strcmp(str, "POP") == 0){
        return POP;
    }

    if (strcmp(str, "RET") == 0){
        return RET;
    }

    else{
        return ERR;
    }
}

commands CheckCommandWithArgs(char *str){

    if (strcmp(str, "PUSH") == 0){
        return PUSH;
    }

    if (strcmp(str, "POP") == 0){
        return POP;
    }

    if(strcmp(str, "POW") == 0){
        return POW;
    }
    if(strcmp(str, "DRAW") == 0){
        return DRAW;
    }

    else{
        printf("ERR?\n");
        return ERR;
    }
}

int CheckArgument(char *str, instruction *instr){

    int argument = 0;

    int checkSscanf = sscanf(str, "%d", &argument);

    if (checkSscanf == 1){
        return argument;
    }

    else{
        char reg[3];

        sscanf(str, "%s", reg);

        registers tempReg = IsRegister(reg);

        if (instr->cmd == PUSH){      

            if (tempReg){
                
                instr->cmd = PUSHR;
                
                return tempReg;

            }
            else{

                instr->cmd = ERR;

                return RERR;

            }
        }
        if (instr->cmd == POP){    

            if (tempReg){

                instr->cmd = POPR;
                
                return tempReg;

            }

            else{

                instr->cmd = ERR;

                return RERR;

            }
        }
    }
    return RERR;
}

registers IsRegister(char *str){

    if (strcmp(str, "AX") == 0){
        return RAX;
    }

    if (strcmp(str, "BX") == 0){
        return RBX;
    }

    if (strcmp(str, "CX") == 0){
        return RCX;
    }

    if (strcmp(str, "DX") == 0){
        return RDX;
    }

    else{
        return RERR;
    }
}

commands CheckCommandJump(char *str){
    if(strcmp(str, "JMP") == 0){
        return JMP;
    }
    if(strcmp(str, "JA") == 0){
        return JA;
    }
    if(strcmp(str, "JAE") == 0){
        return JAE;
    }
    if(strcmp(str, "JB") == 0){
        return JB;
    }
    if(strcmp(str, "JBE") == 0){
        return JBE;
    }
    if(strcmp(str, "JE") == 0){
        return JE;
    }
    if(strcmp(str, "JSUN") == 0){
        return JSUN;
    }
    if(strcmp(str, "CALL") == 0){
        return CALL;
    }
    else{
        return ERR;
    }
}

commands CheckCommandMemory(char *str){
    if(strcmp(str, "PUSH") == 0){
        return PUSHM;
    }
    if(strcmp(str, "POP") == 0){
        return POPM;
    }
    else{
        return ERR;
    }
}

void CreateCodeFile(strInfo *programInfo, assembler *ass){

    programInfo->fileOut = fopen(fileOutName, "w");

    for(int i = 0; i < ass->byteCodeLen; i++){
        fprintf(programInfo->fileOut, "%d ", ass->byteCode[i]);
    }
    
    fclose(programInfo->fileOut);
}

void AssemblerDestructor(strInfo *programInfo, assembler *ass){

    StrHandlerDestroy(programInfo);

    free(ass->byteCode);
    free(ass);
    free(programInfo);
}

void SetLabelMinusOne(assembler *ass){
    memset(ass->Label, -1, sizeof(int) * labelSize);
}