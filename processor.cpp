#include <stdio.h>
#include "stack.cpp"
#include "strhandler.hpp"

struct processor{
    stack_t *stackFrame;
    int *code;
    int IP;
};

int main(){

    strInfo *env;
    processor *APM;

    ProcessorConstructor(&env, &APM, "code.txt");

    OpenInFile(env);

    ReadInFile(env);

    

    return 0;
}

void ProcessorConstructor(strInfo **programInfo, processor **APM, const char *fileName){

    *programInfo = (strInfo *)calloc(1, sizeof(strInfo));

    (*programInfo)->fileName = fileName;

    *APM = (processor *)calloc(1, sizeof(processor));

    (*APM)->stackFrame = StackConstructor(globalMinCapacity, "stackFrame");

}