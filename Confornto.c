//FUNZIONA VERSIONE CON I CICLI FOR SPERANDO CHE LE LUNGHEZZE DELLE PAROLE SIA CORTA

#include <stdio.h>
#include <stdlib.h>

#define N 16

int f = N-1;

typedef struct{
    int ex;
    char *per;
    char *no;
    int esatto;
    int min;
    int letto;
}filtro;

filtro diz[64];

void init(){
    
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        diz[i].per = malloc(sizeof(char)*f);
        diz[i].no = malloc(sizeof(char)*f);
        for(int j = 0; j<f+1;j++){
            diz[i].per[j] = '.';
            diz[i].no[j] = '.';
        }
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
    char str1[N];
    char str2[N];
    char out[N];

    scanf("%s", str2);

    //while(!feof(stdin)){
    scanf("%s", str1);

    for(int i = 0; i<N-1;i++){
        if(str1[i] == str2[i]) out[i] = '+';
        else{
            for(int j = 0; j<N-1;j++){
                if(str1[i] == str2[j] && i!=j) { out[i] = '|'; break;}
            }
            if(out[i] != '|') out[i] = '/';
        }
    }

    for(int i = 0; i<N-1; i++){
        if(out[i]=='|'){
            int cont = 0;
            int giu = 0;
            char c = str1[i];
            for(int j = 0; j<N-1;j++){
                if(str2[j]== str1[i]) cont ++;
                if(str1[j] == c && out[j] == '+') giu ++;
            }
            cont = cont - giu;
            giu = 0;
            for(int k = i-1; k>=0; k--){
                if(str1[k] == c && out[k] == '|') giu ++;
            }
            if(giu >= cont || cont == 0) out[i] = '/';
        }
    }
    printf("%s\n", out);
    init(); 
    /*
    //arrivati alla codifica
    init();
    //ALGORITMO DI RICORDO
    
    for(int i = 0; i<k;k++){
        if(out[i]=='/' && !diz[posizione(str1[i])].letto){
            int pos = 0;
            int max = 0;
            diz[posizione(str1[i])].letto = 1;

            for(int j = i+1; j<k;j++){
                if(str1[j]==str1[i]) {
                    if((out[j]!='/')) {pos = 1; max++;}
                    if(out[j]=='+') *((diz[posizione(str1[i])].per)+i)=str1;
                }
            }
            if(pos != 0){
                //esiste ma in posizione sbagliata
                diz[posizione(str1[i])].ex=1;
                *((diz[posizione(str1[i])].no)+i)=str1;
                //elemento max
                diz[posizione(str1[i])].esatto=max;
            }else{
                diz[posizione(str1[i])].ex=0;
            }
        }else if(!diz[posizione(str1[i])].letto){
            if(out[i]=='+'){
                diz[posizione(str1[i])].ex=1;
                *((diz[posizione(str1[i])].per)+i)=str1;
            }else if(out[i]=='|'){
                diz[posizione(str1[i])].ex=1;
                *((diz[posizione(str1[i])].no)+i)=str1;
            }
        }
    }
    */
   // / -> non esiste o è in di più
   // | -> essite ma non li 
   // + -> esiste, li 
    for(int i = 0; i<f;i++){
        if(!diz[posizione(str1[i])].letto){
            //printf("Dentro a letto\n");
            int pos = 0;
            int max = 0;
            int sl = 0;
            diz[posizione(str1[i])].letto = 1;
            
            for(int j = i+1; j<f;j++){
                if(str1[j]==str1[i]) {
                    if((out[j]!='/')) { pos = 1; max++;}
                    if(out[j]=='|') diz[posizione(str1[i])].no[j]=str1[j];
                    if(out[j]=='/') {sl = 1; diz[posizione(str1[i])].no[j]=str1[j];}
                    if(out[j]=='+') {
                        //printf("Dentro a +\n");
                        //printf("Carattere %c, posizione %d diz %d\n", str1[i], i,posizione(str1[i]));
                        diz[posizione(str1[i])].per[j]=str1[j];
                        //printf("Fine a +\n");
                    }
                }
            }
            if(out[i]=='+') {
                diz[posizione(str1[i])].per[i]=str1[i];
                diz[posizione(str1[i])].ex = 1;
                if(sl && pos){
                    diz[posizione(str1[i])].esatto = max+1;
                }
            }
            else if(out[i]=='/'){
                if(pos) {
                    diz[posizione(str1[i])].no[i]=str1[i];
                    diz[posizione(str1[i])].esatto = max;
                    diz[posizione(str1[i])].ex = 1;
                }else diz[posizione(str1[i])].ex = 0;
            }else if(out[i]=='|'){
                diz[posizione(str1[i])].ex = 1;
                if(!sl) {diz[posizione(str1[i])].min = max+1;}
                else {diz[posizione(str1[i])].esatto = max+1;}
                diz[posizione(str1[i])].no[i]=str1[i];
            }
        }
    }

    for(int i = 0; i<64;i++) {diz[i].letto = 0;}
    
    
    for(int i = 0;i<64;i++){
        if(diz[i].ex != -1){
            printf("carattere pos %d: esiste %d\n", i,diz[i].ex);
            printf("esatto: %d\n", diz[i].esatto);
            printf("min: %d\n", diz[i].min);
            printf("letto: %d\n", diz[i].letto);
            printf("Permesso:\n");
            for(int j = 0; j<f;j++){
                printf("%c",diz[i].per[j]);
            }
            printf("\n");
            printf("NON permesso:\n");
            for(int j = 0; j<f;j++){
                printf("%c",diz[i].no[j]);
            }
            printf("\n");
            printf("_________________\n");
        }
    }
    return 0;

}