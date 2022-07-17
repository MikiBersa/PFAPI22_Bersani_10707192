/*
SPIEGAZIONE DELLA STRUTTRA DEL CODICE:

Strutture dati: 
    1) un albero RB per memorizzare le parole così limito la complessità per la ricerca della parola tra quelle ammissibili 
    2) un dizionario per memorizzare i requisiti appresi durante il confronto con la parola di riferimento
    3) una lista bidirezionale con puntatori alla testa e alla fine della lista per tenere in maniera raggruppata le parole valide cioè con i requisiti appresi con il confronto
    4) eventuali dizionari per il conteggi dei caratteri all'interno della parola

PROBLEMA:
    output corretto negli open ma poi va in timeout nelle prove successive 
    faccio di media 0.066 secondi nel subtask5 dell'open
    sto cercando un modo per ridurre il tempo di esecuzione che per ora non ho trovato
*/


#include <stdio.h>
#include <stdlib.h>

#define DIZ 123 //uso 123 elementi così posso accedere al carattere direttamente (122 è la z ultimo carattere ammissibile)

int k; //lunghezza delle stringhe
int cont_buone = 0; //conteggio delle parole valide
int trovata = 0; //variabile di supporto per vedere se la parola letta durante la partita fa parte delle parole ammissibili


//FACCIO UN ALBERO RB
typedef struct el{
    char *str; //memorizzo la parola / stringa
    int valida; //così so che è valida, cioè rispecchia i requisiti per il +stampa_filtrate e il conteggio
    //LISTA - parole
    struct el *dx; //filgio dx di RB
    //LISTA
    struct el *next; //per la lista di parole valide -> successivo
    //struct el *prev; //per la lista di parole valide -> precedente
} elemento; //nodo del RB e anche della lista

typedef struct{
    int ex; //esiste il carattere nella parola di riferimento
    char *no; //posizioni in cui il carattere non può stare 
    int esatto; //numero esatto del carattere nella parola di riferimento
    int min; //numero minimo del carattere nella parola di riferimento
}filtro; //typedef per il dizionario ogni elemento del dizionario è una lettera

typedef elemento *NodePtr; //puntatore ai nodi di RB

typedef struct{
    NodePtr radice_lista;
    NodePtr fine_lista;
}Root; //testa della lista che punta al primo elemento ed all'ultimo

typedef struct{
    int num; //n° di volte che quella lettera compare nella parola di riferimento
    int cont; //utile dopo per capire quali parole c'è da filtrare
}scan; 

filtro diz[123]; //dizionario della memoria dei carattere appresi e delle loro posizioni utile per il conteggio e +stampa_filtrate
NodePtr diz_lung[123] = {NULL};
scan diz_rif[123]; //memorizzo i carattere della parola di riferimento con le loro occorrenze

NodePtr radice; //definisco il puntatore alla radice
NodePtr lista_prova; //puntatore alla testa della lista
NodePtr TNULL; //TNILL per il RB


//creo lo spazio per memorizzare dove i carattere non possono stare -> provare con i numeri interi
//in primi ho scelto di creare un array di caratteri perchè pesa poco 1B per ogni carattere
void init_diz(char *ver){
    for(int i = 0; i<DIZ;i++){
        if(i>=45){
            diz[i].esatto=0;
            diz[i].min=0;
            diz[i].ex = -1;
            diz[i].no = malloc(sizeof(char)*k);
            for(int j = 0; j<k;j++){
                diz[i].no[j] = '.';
            }
        }else continue;
    }
}

//pulisco il dizionario ad ogni inizio partita perchè la parola di riferimento cambia
void pulisci(char *ver){
    for(int i = 0; i<DIZ;i++){
        if(i>=45 && diz[i].ex!=-1){
            diz[i].esatto=0;
            diz[i].min=0;
            diz[i].ex = -1;
            for(int j = 0; j<k;j++){
                diz[i].no[j] = '.';
            }
        }
        diz_rif[i].cont = 1;
        diz_rif[i].num = 0;
    }
    for(int j = 0; j<k;j++){
        ver[j]='.'; //posti ammissibili del carattere ed obbligati
    }
}

//funzione di supporto per verificare se un carattere è presente in una stringa e in che quantità
//sostituisco le funzioni delle libreria string.h perchè sembrano lente
int presente(char c, char *s, int *conteggio){
    int i = 0;

    for(int m = 0;m<k;m++){
        if(s[m]==c) {
            (*conteggio)++;
            i = 1;
        }
    }
    
    if(i) return 1;
    else return 0; 
}

//funzione per verficare se una stringa del RB o lista rispetta i requisti appena appresi della comparazione con la prola di riferimento
//utile per conteggio e +stampa_filtrate
//complessità visto che il k è costante la complessità di questa funzione è costante sarebbe teta k^2
int validazione(char *conf, char * ver, char * rif){
    int conteggio = 0;
    int conto = 0;

    for(int i = 0;i<k;i++){
        int register num = (int) conf[i]; //uso il register per mantenere la variabile in un registro della cpu così c'è l'ha sempre pronta
        int register num2 = (int) rif[i];

        if(conf[i]!=ver[i] && ver[i]!='.') return 0; //vuol dire che c'è un carattere obbligatorio in quella posizione ma non vi è quel carattere
        else if(diz[num].ex==0) return 0; //non c'è nella parola di riferimento
        else if(diz[num].ex==1){ //vado a vedere le altre caratteristiche
            if(diz[num].no[i]==conf[i]) return 0; //posizione non ammessa
            
            //VERIFICO QUANTE VOLTE SI PRESENTA QUELLA LETTERA
            conteggio = 0;
            for(int j = 0;j<k;j++) {
                if(conf[j]==conf[i]) //ci sono altre lettere nella parola come quella
                    conteggio ++;
            }
            
            //guardo il esatto e minimo
            if(diz[num].esatto!=0 && diz[num].esatto != conteggio) return 0; //numero sbagliato
            else if(diz[num].esatto==0){
                if(diz[num].min!=0 && conteggio <  diz[num].min) return 0; //numero di volte di quel carattere troppo basso
            }

            
        } 

        //VERFICO CON CARATTERI PRESENTI NELLA PAROLA DI RIFERIMENTO appresi durante il confronto
        
        if(diz[num2].ex == 1){
            conto = 0;
            if(!presente(rif[i],conf,&conto))  return 0;
            else{
                if(diz[num2].esatto != 0) {
                    if(conto!=diz[num2].esatto) return 0;
                }else{
                    if(diz[num2].min != 0 && conto <diz[num2].min) return 0; //non è del numero minimo
                }
            }
        } 
    }

    return 1;
}


int uguale(char *c, char *p){
    for(int i = 0;i<k;i++)
        if(c[i] != p[i]) return 0;
    return 1;
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


//LISTA INSERIMENTO IN TESTA IN ORDINE DECRESCENTE siccome vado a leggere le parola in ordine crescende lessicografico
//per avere complessità costante nell'inserimento
//INSERIMENTO ALL'INIZIO
void inserisci_lista(NodePtr *root, NodePtr x, NodePtr ultimo){
    //INSERIMENTO A UNA DIREZIONE
    x->next = NULL;
    if(ultimo!=NULL) ultimo->next = x;
    else *(root) = x;
}

//costante eliminazione in quanto gli passo il nodo stesso
void cancella(NodePtr *root, NodePtr x, NodePtr prec){
    if(prec != NULL){
        prec->next = x->next;
    }else{
        *(root) = x->next;
    }
    /*
    if(x->prev!=NULL) x->prev->next = x->next;
    else (*root).radice_lista = x->next;
    if(x->next != NULL) x->next->prev = x->prev;
    else (*root).fine_lista = x->prev;
    */
}

//pulisco la lista prima di ogni partita visto che la parola di riferimento cambia
void init_lista(NodePtr *l){
    NodePtr p = *l;
    NodePtr prec = NULL;
    //PULIZIA DEI NODI 
    while(p!=NULL) {
        p->valida = 0;
        cancella(&lista_prova, p, prec);
        prec = p; 
        p = p->next;
    }
}

//INSERIMENTO DI UN NUOVO NODO IN LISTA E SE è VALIDO ANCHE NELLA LISTA (VEDA INSERIMENTO DURANTE LA PARTITA)
//non considerando il fix_insert complessità costante con fix_insert teta di ln(n)
//FACCIO CON LA LISTA
//void insert(char *stringa, NodePtr *radicec, int validazione, Root *root, int conf) {
void insert_litsa(char * stringa,NodePtr *radicec,int validazione,NodePtr *root,int conf){
    NodePtr node = malloc(sizeof(elemento)); //creo il posto per il nodo in memoria
    char *st = malloc(sizeof(char)*k); //creo il posto per la parola nel nodo

    NodePtr puntCorrente, puntPrecedente;
    puntPrecedente = NULL;
    puntCorrente = *radicec;
    NodePtr register ult_valido = NULL;

    scrittura(stringa,st); //trasferisco la parola di puntatore
    node->str = st;
	node->dx = NULL; //successivo
    node->valida = validazione; //così lo tengo traccia per dopo

    if(diz_lung[(int) stringa[0]]==NULL) {
        diz_lung[(int) stringa[0]] = node;
        printf("Inserito %s in %c\n", stringa, stringa[0]);
    }
    else{
        if(posizione(node->str, diz_lung[(int) stringa[0]]->str) < 0){
            diz_lung[(int) stringa[0]] = node;
            printf("Inserito dopo %s in %c\n", stringa, stringa[0]);
        }
    }

    puntCorrente = diz_lung[(int) stringa[0]];
    printf("puntCorrente %s della parola %s\n", puntCorrente->str, stringa);
    
    while(puntCorrente!=NULL && posizione(node->str, puntCorrente->str) > 0){
        //ricordo che li scorro in ordine alfabertico dal più piccolo al più grande 
        if(puntCorrente->valida) {
            ult_valido = puntCorrente; //vado a vedere ultimo valido così da inserire la parola in maniera corretta nella lista
        }

        puntPrecedente = puntCorrente;
        puntCorrente = puntCorrente->dx;
    }

    node->dx = puntCorrente; //precede 

    if(puntPrecedente != NULL) puntPrecedente->dx = node;
    else *radicec = node;

    //INSERIMENTO DELLA PAROLA DELLA SECONDA LISTA con +inserisci_inizio
    if(validazione && conf){
        node->next = ult_valido->next; //precede 

        if(ult_valido!= NULL){
            ult_valido->next = node;
        }else{
                //OCCHIO A INIZIO E FINE DELLA LISTA
            *(root) = node;
        }
    }
}

//funzione che conta le parole con i requisiti giusti visti dal confronto
//stampo le parole filtrate anche qui devo comunque riverificare le vecchie valide perchè la nuova parole potrebbe cambiare i requisiti
//complessità teta(n*k^2) con n che diminuisce sempre di più perchè vengono filtrate fino a 1
//fa anche il conteggio
void stampa_lista_filtrato(NodePtr l,char *ver, char *rif,int i){
    NodePtr prec = NULL;
    while(l!=NULL) {
        if(validazione(l->str,ver, rif)){
            l->valida=1;
            prec = l;
            if(i == 0) { cont_buone ++; //printf("%s\n", x->str);
            }
            else {
                //printf("%s\n", l->str);
                puts(l->str);
            }
        }else{
            l->valida=0;
            cancella(&lista_prova, l, prec);
        }

        l = l->next;
    }
}

//funzione che stamap solo le parole valide con i requisiti appresi finora
//viene fatta scorrere la lista complessità max n 
void stampa_lista_filtrato_solo(NodePtr l){
    while(l!=NULL) {
        if(l->valida){
            //printf("%s\n", l->str);
            puts(l->str);
        }
        l = l->next;
    }
}

//avendo letto una sola parola devo leggere tutte le parole e vedere se sono valide se sì inserisco nella lista
//complessità teta(n*k^2) con n totale delle parole inserite all'inizio
//PRIMO INSERIMENTO CARICO LA LISTA DELLA PAROLE VALIDE, prima la lista è vuota
void conto_ordinata_filtrato(NodePtr x,char *ver, char *rif,int i){
    NodePtr ultimo = NULL;
    while(x!=NULL) {
        if(validazione(x->str,ver, rif)){
            x->valida = 1; 
            /*
            printf("Valida %s\n", x->str);
            if(ultimo!=NULL) printf("Ultimo %s\n", ultimo->str);
            else printf("Ultimo NULL\n");
            */
            if(i == 0) {
                cont_buone ++;
            }
            else {
                //printf("%s\n", x->str);
                puts(x->str);
            }
            //ricordo che vado a leggere le parole in ordine CRESCENTE e nella lista le metto in ordine decrescente così comeplssità costante
            //nell'inerimento e costante per leggere dalla fine visto che ho creato un puntatore alla fine
            inserisci_lista(&lista_prova,x, ultimo);
            ultimo = x;
            
        }else{
            x->valida=0;
        }
        x = x ->dx;
    }
}

//stampo tutte le parole di RB
//complessità teta(n) con n numero di parole ammissibili
void inOrder(NodePtr l) {
    //scrittura normale
    while(l!=NULL) {
            //printf("%s\n", l->str);
        puts(l->str);
        l = l->dx;
    }
    /*
		if (node != TNULL) {
			inOrder(node->sx);
			//printf("%s\n", node->str);
            puts(node->str);
			inOrder(node->dx);
		} 
    */
}

void stampa(NodePtr l) {
    //scrittura normale
    while(l!=NULL) {
            //printf("%s\n", l->str);
        puts(l->str);
        l = l->dx;
    }
}
void stampa_valida(NodePtr l) {
    //scrittura normale
    while(l!=NULL) {
            //printf("%s\n", l->str);
        puts(l->str);
        l = l->next;
    }
}
//verifico che la parola letta durante la partita faccia parte della parole ammissibili
//complessità teta(n*k) caso pesso se non è presente
int inOrder_controllo(NodePtr l, char* parola) {
        //GUARDO TUTTA LA LISTA POI VEDIAMO SE ANDARE A SALTI
        while(l!=NULL) {
            //printf("%s\n", l->str);
            if(uguale(l->str,parola)) return 1;
            l  = l->dx;
        }
        return 0;
}

//QUI CREO LA STRINGA RICHIESTA CON I + / |
void confronto(char* str2,char* str1, char *out, char *ver){
    int c; //memorizza il carattere letto
    int diz_conto[DIZ] = {0}; //conteggio di quante volte lo stesso carattere si presentano nella parola
    
    for(int j = 0; j<k;j++){
        //AZZERO PER PORTARE IN PARI IL TUTTO
        diz_rif[(int) str1[j]].cont = 1;
    }

    for(int j = 0; j<k;j++){
        c = (int) str1[j]; //str1 è la parola che leggo
        diz_conto[c] = diz_conto[c] + 1;
        if(str1[j] == str2[j]){ //str2 parola di riferimento
            out[j] = '+'; //posizione corretta e giusto carattere
            diz_rif[c].cont = diz_rif[c].cont + 1; //mi ricordo mante posizioni corrette della stessa carattere utile per / che proviene da |

            //FILTRATO
            ver[j] = (char) str1[j];  //aggiorno i caratteri obbligatori appresi e in che posizione
            diz[c].ex = 1; //dico che esiste

            if(diz_conto[c] > diz[c].min) diz[c].min = diz_conto[c]; //memorizzo che apprendo che il minimo è dato diz_conto
        } 
    }

    for(int i = 0; i<k;i++){
        c = (int) str1[i];
        if(diz_rif[c].num == 0) { 
            out[i] = '/'; //non esiste
            diz[c].ex = 0;
        }
        else {
            if(diz_rif[c].cont <= diz_rif[c].num && out[i] != '+'){
                diz_rif[c].cont = diz_rif[c].cont + 1; 
                out[i] = '|'; //c'è ma in posizione sbagliata
            }else if(diz_rif[c].cont > diz_rif[c].num && out[i] != '+') {
                out[i] = '/'; //c'è ma in eccesso ed in posizione sbagliata
                //QUI SO DI SICURO CHE C'è IL MASSIMO ESATTO
                diz[c].esatto = diz_rif[c].num;
            }
            diz[c].ex = 1;
            if(diz[c].esatto == 0 && diz_conto[c] > diz[c].min) diz[c].min = diz_conto[c]; //aggiorno il minimo
        }

        //PARTE IL FILTRATO
        if(ver[i] != str1[i]) diz[c].no[i]=(char) c;
    }
    
    out[k] ='\0';
    //printf("%s\n", out); //stampo la stringa creata
    puts(out);

}

int main(void){
    char c[4];
    if(fgets(c,4,stdin)!=NULL){}
    k = atoi(&c[0]); //leggo di quanto sono lunghe le parole

    char stringa[k]; //array di supporto per le parole lette
    char rif[k]; //memorizzo la parola di riferimento
    //rif[0] = '&';
    int nuova = 0; //mi identifica inizio di una partita
    int inserimento = 1; //mi identifica se devo fare un inserimento di nuove patole
    int conteggio = 0;  //mi dice con quante parole devo confrontare la massimo 
    int primo_inserimento = 0; //letto la prima parola da confrontare
    int confronto_fatto = 0; //il confronto è stato fatto
    char ver[k+1]; ver[k] = '\0'; //stringa per le posizioni obbligatorie apprese


    //init_rb(); //inizializzo RB
    init_diz(ver); //preparo il dizionario
    radice = NULL;

    while(fgets(stringa,5*k,stdin)!=NULL){ //uso fgets più veloce di fscanf
        if(stringa[0] == '+'){ //caso comandi
            if(stringa[1] == 'n'){
                //INIZIO DI UNA NUOVA PARTITA
                //svuoto la lista proveniente dalla vecchia partita
                if(lista_prova!= NULL) init_lista(&lista_prova);

                lista_prova = NULL;

                nuova = 1;
                inserimento = 0;
                primo_inserimento = 1;

                pulisci(ver); //riazzero il dizionario

                if(fgets(stringa,5*k,stdin)!=NULL){} //leggo la parola di riferimento
                
                for(int i = 0; i<k;i++){
                    rif[i] = stringa[i];
                    diz_rif[(int) rif[i]].num ++; //conto la presenza dei carattere nella parola di riferimento
                }

                if(fgets(stringa,5*k,stdin)!=NULL){}
                conteggio = atoi(&stringa[0]); //conosco quante volte devo al massimo confrontare
                confronto_fatto = 0;

            }
            else if(stringa[1] == 'i'){
                if(stringa[11] == 'i') {
                    inserimento = 1; //leggo comando di inizio inserimento
                }
                else if(stringa[11] == 'f'){
                    inserimento = 0; //leggo comando di fine inserimento
                }
            }
            else if(stringa[1] == 's' && stringa[2] == 't'){
                if(confronto_fatto) {
                    //confronto fatto ma non ancora creato la lista caso in cui confronto la prima parola della nuova partita con la parola di riferimento
                    if(primo_inserimento){ conto_ordinata_filtrato(radice,ver,rif,1);primo_inserimento = 0; }
                    //confronto fatto con lista già creata
                    //stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,1);
                    else stampa_lista_filtrato_solo(lista_prova);
                }else{
                    //SE NON HO ANCORA FATTO UN CONFRONTO STAMPO DIRETTAMENTE TUTTE LE PAROLE DAL RB
                    inOrder(radice);
                }

            }
        }else if(nuova){
            if(inserimento) {
                //inserimento dirante la partita allora conosco già alcuni requisiti delle parole da filtrate
                //mentre inserisco in RB inscerisco anche in lista se risulta essere valida
                insert_litsa(stringa, &radice, validazione(stringa,ver, rif), &lista_prova, confronto_fatto);

            }else{

                //trovata = inOrder_controllo(radice, stringa); //controllo la presenza nelle parole ammissibili
                if(uguale(rif,stringa)){
                        //printf("ok\n"); //uguale 
                        puts("ok");
                        nuova = 0; //FINSICE LA PARTITA
                        conteggio = 0; 
                        primo_inserimento = 1;
                }else if(!inOrder_controllo(radice, stringa)){
                    //printf("not_exists\n"); //non trovata
                    puts("not_exists");
                }else if(conteggio != 1 ){ 
                        
                        char conf[k+1]; //creo array di supporto per il confronto in cui mettere elemento fuori 
                        confronto(rif,stringa, conf,ver);
                        confronto_fatto = 1;
                        conteggio = conteggio - 1; //aggiorno il conteggio
                        cont_buone = 0;

                        if(primo_inserimento) {
                            //printf("Primo inserimento\n");
                            //confronto fatto ma non ancora creato la lista caso in cui confronto la prima parola della nuova partita con la parola di riferimento
                            conto_ordinata_filtrato(radice,ver,rif,0);primo_inserimento = 0;}
                            //conto_ordinata_filtrato(lista.radice,&lista_filtrata,ver,0);primo_inserimento = 0;}
                        else {
                             //confronto fatto con lista già creata, qui devo fare una nuova validazione perchè ho letto una nuova parola
                            stampa_lista_filtrato(lista_prova,ver,rif,0);
                        }
                        printf("%d\n",cont_buone);
                        //printf("STAMPO LA LISTA VALIDA\n");
                        //stampa_valida(lista_prova);

                }else{
                    char conf[k+1];
                    confronto(rif,stringa, conf,ver);
                    cont_buone = 0;
                    //conto_ordinata(radice,ver,rif,0);
                    stampa_lista_filtrato(lista_prova,ver,rif,0);
                    printf("%d\n",cont_buone);
                    puts("ko");
                    nuova = 0; //FINSICE LA PARTITA
                    conteggio = 0;
                    primo_inserimento = 1;
                }
                
            }
        }else if(inserimento) {
            //unserimento prima di ogni partita o alla fine qui imposto di default validazione a 0 perchè non conosco la patola di riferimento
            insert_litsa(stringa, &radice,0, &lista_prova, confronto_fatto);
        }
        //insert_litsa(stringa, &radice,0, &lista_prova, confronto_fatto);
    }

    printf("STAMPO LA LISTA\n");
    stampa(radice);
    printf("STAMPO I PRIMI\n");
    for(int i = 0; i<123;i++){
        if(diz_lung[i]!=NULL) printf("%s carattere %c\n", diz_lung[i]->str, diz_lung[i]->str[0]);
        else printf("NULLO carattere %c\n", (char) i);
    }
    return 0;
}