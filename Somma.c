#include <stdio.h>

int somma(char *prova){
    int som = 0;
    for(int i=0;i<10;i++) som = som + prova[i];
    return som;
}

int main(void){
    char stringa[5];
    int k = 10;

    /*
    int prova[123*k];

    for(int i=0;i<123*k;i++) prova[i] = 0;

    while(fgets(stringa,5*k,stdin)!=NULL){
        printf("%s %d\n", stringa,somma(stringa));
        prova[somma(stringa)+1] ++;
    }

    for(int i=0;i<123*k;i++){
        printf("Posizione %d Valore %d\n", i, prova[i]);
    }*/

    //CONTEGGIO DI QUANTE PAROLE INIZIANO CON LA STESSA LETTERA
    int prova[123] = {0};

    while(fgets(stringa,5*k,stdin)!=NULL){
        printf("%s %d\n", stringa, stringa[0]);
        prova[(int) stringa[0]] ++;
    }

    for(int i=0;i<123;i++){
        printf("Posizione %d Valore %d\n", i, prova[i]);
    }

    return 0;
}