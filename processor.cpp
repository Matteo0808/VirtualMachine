#include <stdio.h>
#include "stack.cpp"
#include "strhandler.hpp"

struct processor{
    stack_t *stackFrame;
    int *code;
    int IP;
};

int main(){

    Info *env;
    processor *APM;

    OpenInFile(env);


    

    return 0;
}

void ProcessorConstructor(Info **programInfo, processor **APM, const char *fileName){

    *programInfo = (Info *)calloc(1, sizeof(Info));

    (*programInfo)->fileName = fileName;

    *APM = (processor *)calloc(1, sizeof(processor));

    (*APM)->stackFrame = StackConstructor(globalMinCapacity, "stackFrame");
    
    
    
}

