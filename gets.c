#include <stdio.h>
#include <string.h>

int main(void){
    char stringa[5];
    int k = 5;
    fgets(stringa,6,stdin);
    printf("stringa %s\n", stringa);
    printf("lunghezza %lu\n", strlen(stringa));
    for(int i = 0;i<k;i++) printf("%c", stringa[i]);
    return 0;
}