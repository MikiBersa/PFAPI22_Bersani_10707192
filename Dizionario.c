#include <stdio.h>



typedef struct{
    int ex;
    char *no;
    int esatto;
    int min;
    int letto;
}filtro;

//così nn ho problemi nell'intercettare il carattere
filtro diz[123];

int main(){
    char str1[5] = "ciao";
    diz[(int) str1[0]].esatto = 24;
    printf("Valore %d", diz['c'].esatto);
    return 0;
}