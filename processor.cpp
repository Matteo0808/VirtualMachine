#include <stdio.h>
#include "stack.hpp"

#define ONE_LINE_BUFFER

#include "strhandler.hpp"
#include "config.hpp"



struct processor{
    stack_t *stackFrame;
    int *code;
    int codeLen;
    int IP;
    stack_t *Label;
    int Registers[5];
    int RAM[192];
};

const int VRAM = 64;



void ProcessorConstructor(processor **APM, strInfo **programInfo, const char *fileName);
void ProcessorDestructor(processor *APM, strInfo *programInfo);

void CreateCode(processor *APM, strInfo *programInfo);
void GetCodeLen(processor *APM, strInfo *programInfo);
void CreateCodeList(processor *APM, strInfo *programInfo);

void Processing(processor *APM);
void DoInstruction(processor *APM);

void cmdOUT(processor *APM);
void cmdPUSH(processor *APM);
void cmdPOP(processor *APM);
void cmdPUSHR(processor *APM);
void cmdPOPR(processor *APM);
void cmdADD(processor *APM);
void cmdSUB(processor *APM);
void cmdMUL(processor *APM);
void cmdDIV(processor *APM);



int main(){

    strInfo *env;

    processor *APM;

    ProcessorConstructor(&APM, &env, "code.txt");

    OpenInFile(env);

    ReadInFile(env);

    CreateCode(APM, env);

DEBUG(
    for(int i = 0; i < APM->codeLen; i++){
        printf("instruction %d: [%d]\n", i, APM->code[i]);
    }
)

    Processing(APM);

    ProcessorDestructor(APM, env);

    return 0;
}

void ProcessorConstructor(processor **APM, strInfo **programInfo, const char *fileName){

    *programInfo = (strInfo *)calloc(1, sizeof(strInfo));

    (*programInfo)->fileName = fileName;

    *APM = (processor *)calloc(1, sizeof(processor));

    (*APM)->IP = 0;

    (*APM)->stackFrame = StackConstructor(globalMinCapacity, "stackFrame");

}

void CreateCode(processor *APM, strInfo *programInfo){

    GetCodeLen(APM, programInfo);    

    CreateCodeList(APM, programInfo);

}

void GetCodeLen(processor *APM, strInfo *programInfo){
    int temp = 0;
    int skip = 0;
    int index = 0;
    char* ptr = programInfo->buffer;

    while(sscanf(ptr, "%d %n", &temp, &skip)){

        index++;

        ptr += skip;
    }

    APM->codeLen = index;

}

void CreateCodeList(processor *APM, strInfo *programInfo){

    APM->code = (int *)calloc(APM->codeLen, sizeof(int));

    int temp = 0;
    int skip = 0;
    int index = 0;
    char* ptr = programInfo->buffer;

    while(sscanf(ptr, "%d %n", &temp, &skip)){

        APM->code[index] = temp;
        index++;

        ptr += skip;
    }
}

void Processing(processor *APM){

    while(APM->code[APM->IP]){
        DoInstruction(APM);
    }

}

void DoInstruction(processor *APM){
    switch (APM->code[APM->IP]){
        case ERR:
            abort();

        case HLT:
            abort();

        case OUT:
            cmdOUT(APM);
            return;

        case PUSH:
            cmdPUSH(APM);
            return;

        case POP:
            cmdPOP(APM);
            return;

        case PUSHR:
            cmdPUSHR(APM);
            return;

        case POPR:
            cmdPOPR(APM);
            return;

        case ADD:
            cmdADD(APM);
            return;

        case SUB:
            cmdSUB(APM);
            return;

        case MUL:
            cmdMUL(APM);
            return;

        case DIV:
            cmdDIV(APM);
            return;

        default:
            abort();
    }
}


void cmdOUT(processor *APM){    
    int out = 0;
    StackPop(APM->stackFrame, &out);

    printf("<<%d>>", out);

    APM->IP++;
}

void cmdPUSH(processor *APM){
    StackPush(APM->stackFrame, &(APM->code[APM->IP + 1]));

    APM->IP += 2;
}

void cmdPOP(processor *APM){
    int *pop = (int *)calloc(1, sizeof(int));
    StackPop(APM->stackFrame, pop);

    APM->IP++;

    free(pop);
}

void cmdPUSHR(processor *APM){
    StackPush(APM->stackFrame, &(APM->Registers[APM->IP + 1]));

    APM->IP += 2;
}

void cmdPOPR(processor *APM){
    StackPop(APM->stackFrame, &(APM->Registers[APM->IP + 1]));

    APM->IP += 2;
}


void cmdADD(processor *APM){
    int elem1 = 0;
    StackPop(APM->stackFrame, &elem1);

    int elem2 = 0;
    StackPop(APM->stackFrame, &elem2);

    int add = elem1 + elem2;
    StackPush(APM->stackFrame, &add);

    APM->IP++;
}

void cmdSUB(processor *APM){
    int elem2 = 0;
    StackPop(APM->stackFrame, &elem2);

    int elem1 = 0;
    StackPop(APM->stackFrame, &elem1);

    int sub = elem1 - elem2;
    StackPush(APM->stackFrame, &sub);

    APM->IP++;
}

void cmdMUL(processor *APM){
    int elem1 = 0;
    StackPop(APM->stackFrame, &elem1);

    int elem2 = 0;
    StackPop(APM->stackFrame, &elem2);

    int mul = elem1 * elem2;
    StackPush(APM->stackFrame, &mul);

    APM->IP++;
}

void cmdDIV(processor *APM){
    int elem2 = 0;
    StackPop(APM->stackFrame, &elem2);

    int elem1 = 0;
    StackPop(APM->stackFrame, &elem1);

    int div = elem1 / elem2;
    StackPush(APM->stackFrame, &div);

    APM->IP++;
}

void ProcessorDestructor(processor *APM, strInfo *programInfo){

    StackDestructor(APM->stackFrame);

    free(APM);

    free(programInfo);
}