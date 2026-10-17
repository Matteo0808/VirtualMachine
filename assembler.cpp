#include <stdio.h>
#include <string.h>

#include "strhandler.hpp"
#include "config.hpp"

struct assembler{
    int *byteCode;
    unsigned int byteCodeLen;
    int instrPtr;
};

struct instruction{
    commands cmd;
    int arg;
};

void AssemblerConstructor(strInfo **programInfo, assembler **ass, const char *fileName);
void AssemblerDestructor(strInfo *programInfo, assembler *ass);

commands CheckCommandNoArgs(char *str);
commands CheckCommandWithArgs(char *str);
int CheckArgument(char *str, instruction *instr);
registers IsRegister(char *str);

void Translator(strInfo *programInfo, assembler *ass);

void CreateCodeFile(strInfo *programInfo, assembler *ass);

int main(){
    
    strInfo *env;
    
    assembler *assembler;

    AssemblerConstructor(&env, &assembler, "code.asm"); //TODO - argv

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

    Translator(env, assembler);

DEBUG(
    for (int i = 0; i < assembler->byteCodeLen; i++){
        printf("%d ", assembler->byteCode[i]);
    }
)

    CreateCodeFile(env, assembler);

    AssemblerDestructor(env, assembler);

    return 0;
}

void AssemblerConstructor(strInfo **programInfo, assembler **ass, const char *fileName){

    *programInfo = (strInfo *)calloc(1, sizeof(strInfo));

    *ass = (assembler *)calloc(1, sizeof(assembler));

    (*ass)->instrPtr = 0;

    (*programInfo)->fileName = fileName;
}

void Translator(strInfo *programInfo, assembler *ass){

    ass->byteCodeLen = 2 * programInfo->arrSize;

    ass->byteCode = (int *)calloc(ass->byteCodeLen, sizeof(int));

    instruction instr = {ERR, RERR};

    for (int index = 0; index < programInfo->arrSize; index++){
                
        char tempStr1[5];
        char tempStr2[11]; //TODO - rename

        int check = sscanf((programInfo->arrLines)[index].line, "%s %s", tempStr1, tempStr2);

        //TODO rename check

        if (check == 1){ //TODO - enum 1 2

            instr.cmd = CheckCommandNoArgs(tempStr1);

            if (instr.cmd == ERR){
                printf("Command error\n");
                abort();
            }
            
            ass->byteCode[ass->instrPtr] = (int)instr.cmd;

            ass->instrPtr++;

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

        DEBUG(
            printf("BIGPRINTF:\nass->instrPtr = %d\ninstr.cmd = %d\ninstr.arg = %d\n\n", ass->instrPtr, instr.cmd, instr.arg);
        )

            ass->byteCode[ass->instrPtr] = instr.cmd;
            ass->byteCode[ass->instrPtr + 1] = instr.arg;

        DEBUG(
            printf("command = <<%d>>, argument = <<%d>>\n", ass->byteCode[ass->instrPtr], ass->byteCode[ass->instrPtr + 1]);
        )

            ass->instrPtr += 2;
        }        
        else{
            instr.cmd = ERR;
        }
    }
}


commands CheckCommandNoArgs(char *str){

    if (strcmp(str, "HLT") == 0){
        return HLT;
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

    else{
        return ERR;
    }
}

commands CheckCommandWithArgs(char *str){

    if (strcmp(str, "PUSH") == 0){
        return PUSH;
    }

    if (strcmp(str, "POP") == 0){
        return POPR;
    }

    else{
        return ERR;
    }
}

int CheckArgument(char *str, instruction *instr){

    int tempInt = 0; //TODO rename

    int check = sscanf(str, "%d", &tempInt);

    if (check == 1){
        return tempInt;
    }

    else{
        char tempStr[3];

        sscanf(str, "%s", tempStr);

        registers tempReg = IsRegister(tempStr);

        if (instr->cmd == PUSH){      

            if (tempReg){

                instr->arg = (int)tempReg;

                instr->cmd = PUSHR;

            }
            else{

                instr->cmd = ERR;

            }
        }
        if (instr->cmd == POP){    

            if (tempReg){
                
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

void CreateCodeFile(strInfo *programInfo, assembler *ass){

    programInfo->fileOut = fopen("code.txt", "w");

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