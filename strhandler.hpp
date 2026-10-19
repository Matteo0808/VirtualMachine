#ifndef STRHANDLER_H
#define STRHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(...) __VA_ARGS__
#else
#define DEBUG(...)
#endif



enum strErrorList{
    RETURN_POISON = -1,
    RETURN_STR_SUCCESS = 0,
    RETURN_STR_ERROR = 1,
    RETURN_ERROR_FILEOPEN = 2,
    RETURN_ERROR_FSTAT = 3,
    RETURN_ERROR_FILEREAD = 4,
    RETURN_ERROR_FILECREATE = 5,
    RETURN_ERROR_FILEWRITE_SORT_BEG = 6,
    RETURN_ERROR_TOO_LARGE_FILE_SIZE = 7,
    RETURN_ERROR_MEMORY_ALLOCATION = 8,
    RETURN_ERROR_FILEADD_SORT_END = 9,
    RETURN_ERROR_FILEWRITE_SORT_END = 10,
    RETURN_ERROR_FILEADD_UNS = 11,
    RETURN_ERROR_FILEWRITE_UNS = 12,
};



struct Line{
    char *line;
    size_t len;
};

struct strInfo{
    const char *fileName;
    int fileInDescriptor;
    FILE *fileOut;
\
    char *buffer;
    size_t bufSize;
\
    Line* arrLines;
    size_t arrSize;
\
    strErrorList error;
};



strErrorList OpenInFile(strInfo *programInfo);
strErrorList ReadInFile(strInfo *programInfo);
strErrorList BufferToLinesFragmentation(strInfo *programInfo);

void PrintfError(strErrorList error);

void ChangeEndlsToNulls(strInfo *programInfo);
void ChangeNullsToEndls(strInfo *programInfo);

void StrHandlerDestroy(strInfo *programInfo DEBUG(, int fileOutUnsortedDescriptor, int fileOutInOneLineDescriptor));



void PrintfError(strErrorList error){
    switch (error){

        case (RETURN_POISON):
            printf("Poison value, the struct has been destroyed\n");
            break;

        case (RETURN_ERROR_FILEOPEN):
            printf("Error in input file opening in fuction open(), maybe you should check console arguments\n");
            break;

        case (RETURN_ERROR_FSTAT):
            printf("Error in input file opening in fuction fstat()\n");
            break;

        case (RETURN_ERROR_FILEREAD):
            printf("Error in input file reading in fuction read()\n");
            break;

        case (RETURN_ERROR_FILECREATE):
            printf("Error in input file creating or opening in fuction fopen() in sorting by beginning, maybe you should check console arguments\n");
            break;

        case (RETURN_ERROR_FILEWRITE_SORT_BEG):
            printf("Error in input file writing in fuction fprintf() in sorting by beginning\n");
            break;

        case (RETURN_ERROR_TOO_LARGE_FILE_SIZE):
            printf("Input file is too large\n");
            break;

        case (RETURN_ERROR_MEMORY_ALLOCATION):
            printf("Error in memory allocation\n");
            break;

        case (RETURN_ERROR_FILEADD_SORT_END):
            printf("Error in input file opening in fuction fopen() in sorting by ending, maybe you should check console arguments\n");
            break;

        case (RETURN_ERROR_FILEWRITE_SORT_END):
            printf("Error in input file writing in fuction fprintf() in sorting by ending\n");
            break;

        case (RETURN_ERROR_FILEADD_UNS):
            printf("Error in input file opening in fuction fopen() in outputting unsorted text, maybe you should check console arguments\n");
            break;

        case (RETURN_ERROR_FILEWRITE_UNS):
            printf("Error in input file writing in fuction fwrite() outputting unsorted text\n");
            break;

        default:
            printf("no errors:)");
            break;
    }
}

strErrorList OpenInFile(strInfo *programInfo){
    assert(programInfo != NULL);
    assert(&(programInfo->bufSize) != 0);
    
    programInfo->fileInDescriptor = open(programInfo->fileName, O_RDONLY | O_BINARY);
    if (programInfo->fileInDescriptor == -1) {return RETURN_ERROR_FILEOPEN;}

    struct stat fileStat = {};
    if (fstat(programInfo->fileInDescriptor, &fileStat) == -1) {return RETURN_ERROR_FSTAT;}

    programInfo->bufSize = fileStat.st_size;
    if (programInfo->bufSize >= (size_t)1<<16) {return RETURN_ERROR_TOO_LARGE_FILE_SIZE;};

    //printf("bufSize = %d\n", programInfo->bufSize);

    return RETURN_STR_SUCCESS;
}

strErrorList ReadInFile(strInfo *programInfo){
    assert(programInfo != NULL);

    char *bufferTemp = (char *)calloc(programInfo->bufSize + 2, sizeof(char));

    if (bufferTemp == NULL) {return RETURN_ERROR_MEMORY_ALLOCATION;};
    
    DEBUG(printf("bufTemp = %p\n", bufferTemp);)

    programInfo->buffer = bufferTemp + 1;

#ifndef ONE_LINE_BUFFER

    programInfo->buffer[programInfo->bufSize] = '\n';
    
#endif

    DEBUG(printf("buf = %p\n", programInfo->buffer);)

    if (read(programInfo->fileInDescriptor, programInfo->buffer, programInfo->bufSize) <= 0){
        close(programInfo->fileInDescriptor);

        return RETURN_ERROR_FILEREAD;
    }
    
    close(programInfo->fileInDescriptor);

    //printf("buffer - [%s]\n", programInfo->buffer);

    return RETURN_STR_SUCCESS;
}

strErrorList OutputUnsortedText(strInfo *programInfo){
    assert(programInfo != NULL);
    assert(programInfo->buffer != NULL);

    if(programInfo->fileOut == NULL){
        programInfo->fileOut= fopen("output.txt", "a");
    }

    if (programInfo->fileOut == NULL){
        fclose(programInfo->fileOut);
        return RETURN_ERROR_FILEADD_UNS;
    }

    ChangeNullsToEndls(programInfo);

    fputs("===Unsorted=Text===\n\n", programInfo->fileOut);

    fwrite(programInfo->buffer, sizeof(char), programInfo->bufSize, programInfo->fileOut);

    fputs("\n\n\n", programInfo->fileOut);

    return RETURN_STR_SUCCESS;
}

void ChangeEndlsToNulls(strInfo *programInfo){
    assert(programInfo != NULL);

    size_t endlCount = 0;

    for (size_t index = 0; index < programInfo->bufSize + 1; index++){
        if (programInfo->buffer[index] == '\r'){

            programInfo->buffer[index] = '\0';
            
        }

        if (programInfo->buffer[index] == '\n'){
            
            endlCount++;

            programInfo->buffer[index] = '\0';

            while((index + 1) < programInfo->bufSize && programInfo->buffer[index + 1] == '\n'){

                programInfo->buffer[index + 1] = '\0';

                index++;
            }
        }
    }
    programInfo->arrSize = endlCount;
}

void ChangeNullsToEndls(strInfo *programInfo){

    for (size_t index = 0; index < programInfo->bufSize; index++){

        if (programInfo->buffer[index] == '\0'){

            programInfo->buffer[index] = '\n';
        }
    }
}

strErrorList BufferToLinesFragmentation(strInfo *programInfo){
    assert(programInfo->buffer != NULL);

    DEBUG(printf("buffer = [%s]\n", programInfo->buffer);
          printf("arrLines ptr = %p\n", programInfo->arrLines);
          printf("bufSize = %llu\n", programInfo->bufSize);
         )

    programInfo->arrLines = (Line *)calloc(programInfo->arrSize, sizeof(Line));
    
    if(programInfo->arrLines == NULL){
        return RETURN_ERROR_MEMORY_ALLOCATION;
    }

    int linesIndex = 0;

    for (ssize_t index = -1; index < (ssize_t)(programInfo->bufSize - 1); index++){
        
        if (programInfo->buffer[index] == '\0'){
            while((index + 1) < (ssize_t)programInfo->bufSize && programInfo->buffer[index + 1] == '\0'){
                
                index++;
            }
            programInfo->arrLines[linesIndex].line = programInfo->buffer + (index + 1);

            linesIndex++;            
        }
    }
    for (size_t index = 0; index < programInfo->arrSize; index++){

        if (programInfo->arrLines[index].line == 0){

            programInfo->arrLines[index].len = 0;
        }
        else{
            programInfo->arrLines[index].len = programInfo->arrLines[index + 1].line - programInfo->arrLines[index].line - 1;
        }
    }
    return RETURN_STR_SUCCESS;
}

void StrHandlerDestroy(strInfo *programInfo DEBUG(, int fileOutInOneLineDescriptor, int fileOutUnsortedDescriptor)){
    DEBUG(close(fileOutInOneLineDescriptor);)
    DEBUG(close(fileOutUnsortedDescriptor);)
    
    fclose(programInfo->fileOut);

    free(programInfo->buffer - 1);
    free(programInfo->arrLines);

    programInfo->error = RETURN_POISON;
    programInfo->fileOut = NULL;
}

#endif