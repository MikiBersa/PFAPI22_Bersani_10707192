#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int k;

typedef struct el{
    char *str;
    int color;
    struct el *sx;
    struct el *dx;
    struct el *p;
} elemento;

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
        diz[i].per = malloc(sizeof(char)*k);
        diz[i].no = malloc(sizeof(char)*k);
        for(int j = 0; j<k;j++){
            diz[i].per[j] = '.';
            diz[i].no[j] = '.';
        }
    }
}

int posizione_diz (char c){
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

void filtrato(char * str1, char * out){
    for(int i = 0; i<k;i++){
        if(!diz[posizione_diz(str1[i])].letto){
            //printf("Dentro a letto\n");
            int pos = 0;
            int max = 0;
            int sl = 0;
            diz[posizione_diz(str1[i])].letto = 1;
            
            for(int j = i+1; j<k;j++){
                if(str1[j]==str1[i]) {
                    if((out[j]!='/')) { 
                        pos = 1; //mi dice che escludo il non appartenere
                        max++;
                    }
                    if(out[j]=='|') diz[posizione_diz(str1[i])].no[j]=str1[j];
                    if(out[j]=='/') {
                        sl = 1; //mi dice che ho dei valori esatti
                        diz[posizione_diz(str1[i])].no[j]=str1[j];
                    }
                    if(out[j]=='+') {
                        //printf("Dentro a +\n");
                        //printf("Carattere %c, posizione %d diz %d\n", str1[i], i,posizione(str1[i]));
                        diz[posizione_diz(str1[i])].per[j]=str1[j];
                        //printf("Fine a +\n");
                    }
                }
            }
            if(out[i]=='+') {
                diz[posizione_diz(str1[i])].per[i]=str1[i];
                diz[posizione_diz(str1[i])].ex = 1;
                if(sl && pos){
                    diz[posizione_diz(str1[i])].esatto = max+1;
                }
            }
            else if(out[i]=='/'){
                if(pos) {
                    diz[posizione_diz(str1[i])].no[i]=str1[i];
                    diz[posizione_diz(str1[i])].esatto = max;
                    diz[posizione_diz(str1[i])].ex = 1;
                }else diz[posizione_diz(str1[i])].ex = 0;
            }else if(out[i]=='|'){
                diz[posizione_diz(str1[i])].ex = 1;
                if(!sl) {
                    if((max+1) > diz[posizione_diz(str1[i])].min) diz[posizione_diz(str1[i])].min = max+1; //aggiorno dopo altre letture
                }
                else {diz[posizione_diz(str1[i])].esatto = max+1;}
                diz[posizione_diz(str1[i])].no[i]=str1[i];
            }
        }
    }

    for(int i = 0; i<64;i++) {diz[i].letto = 0;}
}

void scrivi(){
     for(int i = 0;i<64;i++){
        if(diz[i].ex != -1){
            printf("carattere pos %d: esiste %d\n", i,diz[i].ex);
            printf("esatto: %d\n", diz[i].esatto);
            printf("min: %d\n", diz[i].min);
            printf("letto: %d\n", diz[i].letto);
            printf("Permesso:\n");
            for(int j = 0; j<k;j++){
                printf("%c",diz[i].per[j]);
            }
            printf("\n");
            printf("NON permesso:\n");
            for(int j = 0; j<k;j++){
                printf("%c",diz[i].no[j]);
            }
            printf("\n");
            printf("_________________\n");
        }
    }
}

int posizione(char* c1, char *c2){ //c2 la testa
    for(int i = 0; i<k;i++){
        if(c1[i] > c2[i]) return 1; //destra
        else if(c1[i] < c2[i]) return -1; //sinistra
    }
    return 0;
}

void scrittura(char* c1, char *c2){ //da c1 a c2
    for(int i = 0; i<k;i++){
        c2[i] = c1[i];
    }
}

int inserimento_tree(elemento * *lista, char* parola){
    elemento * y = NULL;
    elemento * x = *(lista);
    elemento * ell = malloc(sizeof(elemento));
    char *st = malloc(sizeof(char)*k);
    scrittura(parola, st);

    int i = 0;
    while (x != NULL){
        i ++;
        y = x;
        //printf("Stringa di prova: %s\n", (*lista)->str);
        //printf("Parola in x: %s, parola in stringa: %s\n", x->str, st);
        if(posizione(st,x->str) == -1){ x = x->sx; //printf("-1\n");
        }
        else {x = x->dx; //printf("1\n");
        }
    }
    ell->p = y;
    ell->str = st;
    ell->sx = NULL;
    ell->dx = NULL;

    if(y==NULL)
        *(lista) = ell;
    else if(posizione(ell->str, y->str) == -1) {
        //printf("Dentro a sx");
        y->sx=ell;}
    else {y->dx=ell; //printf("Dentro a dx");
    }

    return i;
}

void scrittura_ordinata(elemento *x){
    if(x!=NULL){    
        scrittura_ordinata(x->sx);
        printf("%s\n", x->str);
        scrittura_ordinata(x->dx);
    }
}

int uguale(char *c, char *p){
    for(int i = 0;i<k;i++)
        if(c[i] != p[i]) return 0;
    return 1;
}

int controllo(elemento *x, char * parola){
    if(x!=NULL){   
        if(uguale(x->str,parola)) return 1;
        else return (controllo(x->sx, parola) || controllo(x->dx,parola));
    }
    else return 0;
}

char * confronto(char* str2,char* str1){
    char out[k+1]; //DA CONTROLLARE IL FATTO DI FINIRE CON \0
    for(int i = 0; i<=k-1;i++){
        if(str1[i] == str2[i]) out[i] = '+';
        else{
            for(int j = 0; j<=k-1;j++){
                if(str1[i] == str2[j] && i!=j) { out[i] = '|'; break;}
            }
            if(out[i] != '|') out[i] = '/';
        }
    }

    for(int i = 0; i<=k-1; i++){
        if(out[i]=='|'){
            int cont = 0;
            int giu = 0;
            char c = str1[i];
            for(int j = 0; j<=k-1;j++){
                if(str2[j]== str1[i]) cont ++;
                if(str1[j] == c && out[j] == '+') giu ++;
            }
            cont = cont - giu;
            giu = 0;
            for(int l = i-1; l>=0; l--){
                if(str1[l] == c && out[l] == '|') giu ++;
            }
            if(giu >= cont || cont == 0) out[i] = '/';
        }
    }
    out[k] ='\0'; // VERIFICARE CON IL DIFF VEDERE COSA VUOLE
    printf("%s\n", out);
    char * rit = malloc(sizeof(char)*k);
    scrittura(out, rit);
    return rit;
}

int main(void){
    clock_t begin = clock();
    elemento * lista = NULL;
    scanf("%d", &k);
    char stringa[k];
    char rif[k];
    rif[0] = '&';
    int nuova = 0;
    int inserimento = 1;
    int conteggio = 0;

    //init(); //preparo il dizionario

    while(!feof(stdin)){
        scanf("%s", stringa);
        //printf("Letto %s\n",stringa);
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                nuova = 1;
                inserimento = 0;
                //printf("INIZIO_PARTITA in +\n");
                init(); //riazzero il dizionario
                //inserisco i nuovi elementi 
                scanf("%s", stringa);
                scrittura(stringa,rif);
                scanf("%s", stringa);
                conteggio = atoi(&stringa[0]);
                //printf("CONTEGGIO %d\n", conteggio);
            }
            else if(stringa[1] == 'i'){
                if(stringa[11] == 'i') {
                    inserimento = 1;
                }
                else if(stringa[11] == 'f'){
                    inserimento = 0;
                }
            }
            else if(stringa[1] == 's' && stringa[2] == 't'){
                //scrivi();
                printf("S\n");
            }
        }else if(nuova){
            if(inserimento) {
                inserimento_tree(&lista,stringa);
                //printf("Inserito nuove stringhe\n");
            }/*
            //QUI SIAMO DENTRO ALLA PARTITA
            else if(rif[0] == '&'){ //inserimento di una nuova parola -> DA METTERE QUANDO FINISCE UNA PARTITA A RIF[0] = '&'
                
                scrittura(stringa,rif);
                scanf("%s", stringa);
                conteggio = atoi(&stringa[0]);
                printf("INIZIO_PARTITA\n"); //L'HO MESSO IO POI VA TOLTO
                printf("Parola di riferminto %s\n", rif);
                printf("CONTEGGIO: %d\n", conteggio);
                
            }*/
            else{
                //Qui dovrebbe partire algoritmo 
                //printf("Stringhe da confrontare %s\n", stringa);
                //VEDERE SE APPARTIENE O NO ALLE PAROLE AMMISSBILI
                if(uguale(rif,stringa)){
                        printf("ok\n");
                        nuova = 0; //FINSICE LA PARTITA
                        //rif[0] = '&';
                }else if(conteggio != 1){ 
                    if(controllo(lista,stringa)){
                        char *ritorno;
                        ritorno = confronto(rif,stringa);
                        filtrato(stringa,ritorno);
                        free(ritorno);
                        conteggio --;
                    }else{
                        printf("not_exists\n");
                    }
                }else{
                    char *ritorno;
                    ritorno = confronto(rif,stringa);
                    filtrato(stringa,ritorno);
                    free(ritorno);
                    printf("ko\n");
                    nuova = 0; //FINSICE LA PARTITA
                    //rif[0] = '&';
                }
                
            }
        }else if(inserimento) {
            inserimento_tree(&lista,stringa);
            //printf("Inserito nuove stringhe\n");
        }
    }

    //scrittura_ordinata(lista);
    
    
    clock_t end = clock();
    double time_spent=0.0;
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("tempo: %.3f", time_spent);
    
    return 0;
}