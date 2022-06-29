#include <stdio.h>
#define N 5

int k = N;

typedef struct{
    int ex;
    char *per;
    char *no;
    int esatto;
    int min;
}filtro;

filtro diz[64];

void init(){
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        diz[i].per = malloc(sizeof(char)*k);
        diz[i].no = malloc(sizeof(char)*k);
    }
}

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
    char c;
    scanf("%c",&c);
    init();

    
    //diz[posizione(c)] = 23;
    printf("Posizione %d" ,posizione(c));
    return 0;
}