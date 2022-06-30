#include <stdio.h>
#include <stdlib.h>

int k;
int cont_buone = 0;

typedef struct el{
    char *str;
    struct el *sx;
    struct el *dx;
    struct el *p;
} elemento;

typedef struct{
    int ex;
    //char *per;
    char *no;
    int esatto;
    int min;
    int letto;
}filtro;

filtro diz[64];

void init(char *ver){
    
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        //diz[i].per = malloc(sizeof(char)*k);
        diz[i].no = malloc(sizeof(char)*k);
        for(int j = 0; j<k;j++){
            //diz[i].per[j] = '.';
            diz[i].no[j] = '.';
            ver[j]='.';
        }
    }
}

void pulisci(char *ver){
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        for(int j = 0; j<k;j++){
            //diz[i].per[j] = '.';
            diz[i].no[j] = '.';
            ver[j]='.';
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

char inv_posizione (int n){
    if(n==0) return '-';
    if(n==1) return '_';
    if(n>=2 && n<=11) return (char) (n+46);
    if(n>=12 && n<=37) return (char) (n+53);
    if(n>=38 && n<=63) return (char) (n+59);
    return '.';
}

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

int validazione(char *conf, char * ver){
    int conteggio = 0;
    for(int i = 0;i<k;i++){
        if(conf[i]!=ver[i] && ver[i]!='.') return 0; //vuol dire che c'è
        //else if(diz[posizione_diz(conf[i])].ex==-1) continue; //non ancora analizzata
        else if(diz[posizione_diz(conf[i])].ex==0) return 0; //confornto con le posizion obbligate
        else if(diz[posizione_diz(conf[i])].ex==1){ //non c'è 
            if(diz[posizione_diz(conf[i])].no[i]==conf[i]) return 0; //posizione sbagliata
            //if(diz[posizione_diz(conf[i])].per[i]!='.' && conf[i]!=diz[posizione_diz(conf[i])].per[i]) return 0; //non è quella obbligatoria
            for(int j = 0;j<k;j++) {
                if(conf[j]==conf[i]) //ci sono altre lettere nella parola come quella
                    conteggio ++;
            }

            if(diz[posizione_diz(conf[i])].esatto!=0 && diz[posizione_diz(conf[i])].esatto != conteggio) return 0;
            else if(diz[posizione_diz(conf[i])].esatto==0){
            if(diz[posizione_diz(conf[i])].min!=0 && conteggio <  diz[posizione_diz(conf[i])].min) return 0;
            }
        }


    }

    //ANALISI DELLE PAROLE CHE NON HA QUELLA PAROLA MA CHE ALTRE HANNO
    for(int l = 0; l<64;l++){
        if(diz[l].ex==1){ 
            char car = inv_posizione(l);
            if(!presente(car,conf,0))  return 0; //proprio non è presente
            else{
                if(diz[l].min != 0 && presente(car,conf,1)<diz[l].min) return 0; //non è del numero minimo
                if(diz[l].esatto != 0 && presente(car,conf,1)!=diz[l].esatto) return 0;
            }
        }
    }

    return 1;
}

void filtrato(char * str1, char * out, char * ver){
    for(int i = 0; i<k;i++){
        if(!diz[posizione_diz(str1[i])].letto && diz[posizione_diz(str1[i])].ex!=0){
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
                        //diz[posizione_diz(str1[i])].per[j]=str1[j];
                        //printf("Fine a +\n");
                        ver[j] = str1[j];
                    }
                }
            }
            if(out[i]=='+') {
                ver[i] = str1[i];
                /*
                if(diz[posizione_diz(str1[i])].per[i]=='.'){
                    diz[posizione_diz(str1[i])].per[i]=str1[i];
                    //diz[posizione_diz(str1[i])].min = diz[posizione_diz(str1[i])].min + 1;
                }*/
                
                diz[posizione_diz(str1[i])].ex = 1;
                if(sl && pos){
                    diz[posizione_diz(str1[i])].esatto = max+1;
                }

                if((max+1) > diz[posizione_diz(str1[i])].min) diz[posizione_diz(str1[i])].min = max+1;
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
/*
void scrittura_ordinata(elemento *x,char *ver){
    if(x!=NULL){    
        scrittura_ordinata(x->sx,ver);
        if(validazione(x->str,ver)) printf("%s\n", x->str);
        scrittura_ordinata(x->dx,ver);
    }
}
*/

void conto_ordinata(elemento *x,char *ver, int i){
    if(x!=NULL){    
        conto_ordinata(x->sx,ver,i);
        if(validazione(x->str,ver)){
            if(i == 0) cont_buone ++;
            else printf("%s\n", x->str);
        }
        conto_ordinata(x->dx,ver,i);
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
    //printf("stringa: %s %s\n",str1, out);
    printf("%s\n", out);
    char * rit = malloc(sizeof(char)*k);
    scrittura(out, rit);
    return rit;
}

void scrivi(char *ver){
    printf("---------------\n");
    printf("Verificato: %s \n", ver);
    for(int i = 0;i<64;i++){
        if(diz[i].ex != -1){
            printf("carattere pos %d: esiste %d  car: %c\n", i,diz[i].ex, inv_posizione(i));
            printf("esatto: %d\n", diz[i].esatto);
            printf("min: %d\n", diz[i].min);
            printf("letto: %d\n", diz[i].letto);
            printf("Permesso:\n");
            /*
            for(int j = 0; j<k;j++){
                printf("%c",diz[i].per[j]);
            }*/
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

int main(void){
    elemento * lista = NULL;
    if(scanf("%d", &k) != EOF);
    char stringa[k];
    char rif[k];
    rif[0] = '&';
    int nuova = 0;
    int inserimento = 1;
    int conteggio = 0;
    char ver[k+1]; ver[k] = '\0';

    init(ver); //preparo il dizionario

    while(!feof(stdin)){
        if(scanf("%s", stringa)!=EOF);
        //printf("Letto %s\n",stringa);
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                nuova = 1;
                inserimento = 0;
                //printf("INIZIO_PARTITA in +\n");
                pulisci(ver); //riazzero il dizionario
                //inserisco i nuovi elementi 
                if(scanf("%s", stringa)!=EOF);
                scrittura(stringa,rif);
                if(scanf("%s", stringa)!=EOF);
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
                conto_ordinata(lista,ver,1);
                //printf("S\n");
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
                        filtrato(stringa,ritorno, ver);
                        free(ritorno);
                        conteggio --;
                        //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                        cont_buone = 0;
                        conto_ordinata(lista,ver,0);
                        printf("%d\n",cont_buone);
                        //conto_ordinata(lista,ver,1);
                        //scrivi(ver);
                    }else{
                        printf("not_exists\n");
                    }
                }else{
                    char *ritorno;
                    ritorno = confronto(rif,stringa);
                    filtrato(stringa,ritorno, ver);
                    free(ritorno);
                    //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                    cont_buone = 0;
                    conto_ordinata(lista,ver,0);
                    printf("%d\n",cont_buone);
                    
                    printf("ko\n");
                    //conto_ordinata(lista,ver,1);
                    //scrivi(ver);
                    pulisci(ver);
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
    
    return 0;
}