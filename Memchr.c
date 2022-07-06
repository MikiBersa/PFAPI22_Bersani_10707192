#include <stdio.h>
#include <string.h>

int k = 5;

int presente(char c, char *s, int tipo){
    
    if(tipo == 0) {
        for(int m = 0;m<k;m++){
            if(s[m]==c) return 1;
        }
        return 0;
    }else if(tipo == 1){
        int cont=0;
        for(int m = 0;m<k;m++){
            if(s[m]==c) cont++;
        }
        return cont;
    }
    return -1;
}

int posizione(char* c1, char *c2){ //c2 la testa
    for(int i = 0; i<k;i++){
        if(c1[i] > c2[i]) return 1; //destra
        else if(c1[i] < c2[i]) return -1; //sinistra
    }
    return 0;
}

int uguale(char *c, char *p){
    for(int i = 0;i<k;i++)
        if(c[i] != p[i]) return 0;
    return 1;
}

void scrittura(char* c1, char *c2){ //da c1 a c2
    for(int i = 0; i<k;i++){
        c2[i] = c1[i];
    }
}

int main(void){
    char stringa[5];
    char stringa2[5];
    char const c[2] = {'c'};
    while(fgets(stringa,5,stdin)!=NULL){
    if(fgets(stringa2,5,stdin)!=NULL){}
    
    if(memchr(stringa, c[0], 5)!=NULL) printf("Presente\n");
    else printf("Non Presente\n");

    printf("Prova %d\n", memcmp(stringa,stringa2,5));
    memcpy(stringa2, stringa, 5);
    printf("Copia %s\n", stringa);
    
    
    /*
    if(strstr(stringa, c)!=NULL) printf("Presente\n");
    else printf("Non Presente\n");

    printf("Prova %d", strcmp(stringa,stringa2));
    strncpy(stringa2, stringa,5);
    printf("Copia %s\n", stringa2);
    */

    /*
    if(presente(c[0],stringa, 0)) printf("Presente\n");
    else printf("Non Presente\n");

    printf("Prova %d\n", posizione(stringa,stringa2));
    scrittura(stringa2, stringa);
    printf("Copia %s\n", stringa);

    
    */
    }
    return 0;
}