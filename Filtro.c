#include <stdio.h>
#define N 5

int k = N;

typedef struct{
    int ex;
    int *per;
    int *no;
    int esatto;
    int min;
}filtro;

int posizione (char c){
    if(c==45) return 0;
    else if(c==95) return 1;
    else if(c>=48 && c<= 57){
        return c-46;
    }else if(c>= 65 && c<= 90){
        return c-53;
    }else if(c>= 97 && c<= 122){
        return c-59;
    }
    return -1;
}

int main(void){
    filtro diz[64];
    char c;
    scanf("%c",&c);
    //diz[posizione(c)] = 23;
    printf("Posizione %d" ,posizione(c));
    return 0;
}