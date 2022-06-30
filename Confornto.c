//FUNZIONA VERSIONE CON I CICLI FOR SPERANDO CHE LE LUNGHEZZE DELLE PAROLE SIA CORTA

#include <stdio.h>
#include <stdlib.h>

#define N 6

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
char ver[N];

void init(){
    
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        diz[i].per = malloc(sizeof(char)*f);
        diz[i].no = malloc(sizeof(char)*f);
        for(int j = 0; j<f;j++){
            diz[i].per[j] = '.';
            diz[i].no[j] = '.';
            ver[j]='.';
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
        for(int m = 0;m<f;m++){
            if(s[m]==c) return 1;
        }
        return 0;
    }else{
        int cont=0;
        for(int m = 0;m<f;m++){
            if(s[m]==c) cont++;
        }
        return cont;
    }
}

int validazione(char *conf){
    int conteggio = 0;
    for(int i = 0;i<f;i++){
        if(conf[i]!=ver[i] && ver[i]!='.') return 0; //vuol dire che c'è
        else if(diz[posizione(conf[i])].ex==-1) continue; //non ancora analizzata
        else if(diz[posizione(conf[i])].ex==0) return 0; //confornto con le posizion obbligate
        else if(diz[posizione(conf[i])].ex==1){ //non c'è 
            if(diz[posizione(conf[i])].no[i]==conf[i]) return 0; //posizione sbagliata
            if(diz[posizione(conf[i])].per[i]!='.' && conf[i]!=diz[posizione(conf[i])].per[i]) return 0; //non è quella obbligatoria
            for(int j = 0;j<f;j++) {
                if(conf[j]==conf[i]) //ci sono altre lettere nella parola come quella
                    conteggio ++;
            }
        }

        if(diz[posizione(conf[i])].esatto!=0 && diz[posizione(conf[i])].esatto != conteggio) return 0;
        else if(diz[posizione(conf[i])].esatto==0){
            if(diz[posizione(conf[i])].min!=0 && conteggio <  diz[posizione(conf[i])].min) return 0;
        }

    }

    //ANALISI DELLE PAROLE CHE NON HA QUELLA PAROLA MA CHE ALTRE HANNO
    for(int l = 0; l<64;l++){
        if(diz[l].ex==1){ 
            if(!presente(inv_posizione(l),conf,0))  return 0; //proprio non è presente
            else{
                if(presente(inv_posizione(l),conf,1)<diz[l].min) return 0; //non è del numero minimo
            }
        }
    }

    return 1;
}

void scrivi(){
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
}

int main(void){
    char str1[N];
    char str2[N];
    char out[N];

    int cont = 0;
    scanf("%s", str2);
    init(); 
    while(!feof(stdin) && cont!=4){
    //while(!feof(stdin)){
    scanf("%s", str1);
    cont ++;
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
    printf("stringa: %s traduzione: %s\n", str1,out);
   // / -> non esiste o è in di più
   // | -> essite ma non li 
   // + -> esiste, li 
   //printf("PRIMA:\n");
   //scrivi();
    for(int i = 0; i<f;i++){
        if(!diz[posizione(str1[i])].letto){
            //printf("Dentro a letto\n");
            int pos = 0;
            int max = 0;
            int sl = 0;
            diz[posizione(str1[i])].letto = 1;
            
            for(int j = i+1; j<f;j++){
                if(str1[j]==str1[i]) {
                    if((out[j]!='/')) { 
                        pos = 1; //mi dice che escludo il non appartenere
                        max++;
                    }
                    if(out[j]=='|') diz[posizione(str1[i])].no[j]=str1[j];
                    if(out[j]=='/') {
                        sl = 1; //mi dice che ho dei valori esatti
                        diz[posizione(str1[i])].no[j]=str1[j];
                    }
                    if(out[j]=='+') {
                        //printf("Dentro a +\n");
                        //printf("Carattere %c, posizione %d diz %d\n", str1[i], i,posizione(str1[i]));
                        diz[posizione(str1[i])].per[j]=str1[j];
                        ver[j] = str1[j];
                        //printf("Fine a +\n");
                    }
                }
            }
            if(out[i]=='+') {
                ver[i] = str1[i];
                if(diz[posizione(str1[i])].per[i]=='.'){
                    diz[posizione(str1[i])].per[i]=str1[i];
                    diz[posizione(str1[i])].min = diz[posizione(str1[i])].min + 1;
                }
                diz[posizione(str1[i])].ex = 1;
                if(sl && pos){
                    diz[posizione(str1[i])].esatto = max+1;
                }
                if((max+1) > diz[posizione(str1[i])].min) diz[posizione(str1[i])].min = max+1;
            }
            else if(out[i]=='/'){
                if(pos) {
                    diz[posizione(str1[i])].no[i]=str1[i];
                    diz[posizione(str1[i])].esatto = max;
                    diz[posizione(str1[i])].ex = 1;
                }else diz[posizione(str1[i])].ex = 0;
            }else if(out[i]=='|'){
                diz[posizione(str1[i])].ex = 1;
                if(!sl) {
                    if((max+1) > diz[posizione(str1[i])].min) diz[posizione(str1[i])].min = max+1; //aggiorno dopo altre letture
                }
                else {diz[posizione(str1[i])].esatto = max+1;}
                diz[posizione(str1[i])].no[i]=str1[i];
            }
        }
    }

    for(int i = 0; i<64;i++) {diz[i].letto = 0;}
    
    //printf("DOPO:\n");
    //scrivi();
    

   //ALGORITMO PER FILTRARE



    }

    
    while(!feof(stdin)){
        scanf("%s", str1);
        printf("Paorla: %s valida %d\n", str1,validazione(str1));
    }
    
    return 0;

}