#include <stdio.h>
#include <stdlib.h>

int k;

typedef struct el{
    char *str;
    int color;
    struct el *sx;
    struct el *dx;
    struct el *p;
} elemento;

int posizione(char* c1, char *c2){ //c2 la testa
    for(int i = 0; i<k;i++){
        if(c1[i] > c2[i]) return 1; //destra
        else if(c1[i] < c2[i]) return -1; //sinistra
    }
    return 0;
}

void scrittura(char* c1, char *c2){
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

void confronto(char* str2,char* str1){
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
}

int main(void){
    elemento * lista = NULL;
    scanf("%d", &k);
    char stringa[k];
    char rif[k];
    rif[0] = '&';
    int nuova = 0;
    int inserimento = 1;
    int conteggio = 0;

    while(!feof(stdin)){
        scanf("%s", stringa);
        //printf("Letto %s\n",stringa);
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                nuova = 1;
                inserimento = 0;
            }
            if(stringa[1] == 'i'){
                if(stringa[11] == 'i') {
                    inserimento = 1;
                }
                else{
                    inserimento = 0;
                }
            }
        }
        else if(!nuova || inserimento) inserimento_tree(&lista,stringa);
        else if(nuova){
            //QUI SIAMO DENTRO ALLA PARTITA
            if(rif[0] == '&'){ //inserimento di una nuova parola -> DA METTERE QUANDO FINISCE UNA PARTITA A RIF[0] = '&'
                scrittura(stringa,rif);
                scanf("%s", stringa);
                conteggio = atoi(&stringa[0]);
                printf("INIZIO_PARTITA\n");
            }
            else{
                //Qui dovrebbe partire algoritmo 
                //printf("Stringhe da confrontare %s\n", stringa);
                //VEDERE SE APPARTIENE O NO ALLE PAROLE AMMISSBILI
                if(controllo(lista,stringa)){
                    confronto(rif,stringa);
                }else{
                    printf("not_exists\n");
                }
                
            }
        }
    }

    scrittura_ordinata(lista);
    printf("Parola di riferminto %s\n", rif);
    printf("CONTEGGIO: %d", conteggio);

    return 0;
}