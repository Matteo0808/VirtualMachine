#include <stdio.h>
#include <stdlib.h>



int main(){

    const char *str = "PUSH AX";

    char *tempStr = (char *)calloc(20, 1);
    int tempInt = 0;

    int check = sscanf(str, "%s %s", tempStr, &tempInt);

    printf("%d, %s, %d\n", check, tempStr, tempInt);

    return 0;
}

