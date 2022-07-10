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
    int colore; //0 è nero 1 è rosso
    int valida; //così so che è valida, cioè rispecchia i requisiti per il +stampa_filtrate e il conteggio
    struct el *sx; //filgio sx di RB
    struct el *dx; //filgio dx di RB
    struct el *p; //padre di RB
    struct el *next; //per la lista di parole valide -> successivo
    struct el *prev; //per la lista di parole valide -> precedente
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
scan diz_rif[123]; //memorizzo i carattere della parola di riferimento con le loro occorrenze

NodePtr radice; //definisco il puntatore alla radice
Root lista_prova; //puntatore alla testa della lista
NodePtr TNULL; //TNILL per il RB

//inizializzo il TNILL utile per RB
void init_rb(){
    TNULL = malloc(sizeof(elemento));
    TNULL->colore=0; //impongo che sia nero come da regole RB
    TNULL->sx=NULL;
    TNULL->dx=NULL;
    radice = TNULL; //tutte le foglie compresa la radice devono puntare a TNILL
}

//creo lo spazio per memorizzare dove i carattere non possono stare -> provare con i numeri interi
//in primi ho scelto di creare un array di caratteri perchè pesa poco 1B per ogni carattere
void init(char *ver){
    
    for(int i = 0; i<DIZ;i++){
        diz[i].no = malloc(sizeof(char)*k);
    }
}

//pulisco il dizionario ad ogni inizio partita perchè la parola di riferimento cambia
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
void pulisci(char *ver){
    for(int i = 0; i<DIZ;i++){
        if(i>=45){
            diz[i].esatto=0;
            diz[i].min=0;
            diz[i].ex = -1;
            diz_rif[i].cont = 1;
            diz_rif[i].num = 0;
            for(int j = 0; j<k;j++){
                diz[i].no[j] = '.';
            }
        }
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
            
            //VERIFICO QUENATE VOLTE SI PRESENTA QUELLA LETTERA
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
void inserisci_lista(Root *root, NodePtr x){
    x->next = root->radice_lista;
    if(root->radice_lista!=NULL) root->radice_lista->prev = x;
    if(root->radice_lista==NULL) root->fine_lista = x;
    root->radice_lista = x;
    x->prev = NULL;
}

//costante eliminazione in quanto gli passo il nodo stesso
void cancella(Root *root, NodePtr x){
    if(x->prev!=NULL) x->prev->next = x->next;
    else (*root).radice_lista = x->next;
    if(x->next != NULL) x->next->prev = x->prev;
    else (*root).fine_lista = x->prev;
}

//pulisco la lista prima di ogni partita visto che la parola di riferimento cambia
void init_lista(Root *l){
    NodePtr p = l->radice_lista;
    //PULIZIA DEI NODI 
    while(p!=NULL) {
        p->valida = 0;
        cancella(&lista_prova, p); 
        p = p->next;
    }
}

//RB rotazione a dx
void rightRotate(NodePtr x, NodePtr *radicec) {
		NodePtr y = x->sx;
		x->sx = y->dx;
		if (y->dx != TNULL) {
			y->dx->p = x;
		}
		y->p = x->p;
		if (x->p == NULL) {
			*(radicec) = y;
		} else if (x == x->p->dx) {
			x->p->dx = y;
		} else {
			x->p->sx = y;
		}
		y->dx = x;
		x->p = y;
	}

//RB rotazione a sx
void leftRotate(NodePtr x, NodePtr *radicec) {
		NodePtr y = x->dx;
		x->dx = y->sx;
		if (y->sx != TNULL) {
			y->sx->p = x;
		}
		y->p = x->p;
		if (x->p == NULL) {
			*(radicec) = y;
		} else if (x == x->p->sx) {
			x->p->sx = y;
		} else {
			x->p->dx = y;
		}
		y->sx = x;
		x->p = y;
	}

//RB correzione inserimento per mantenere le regole rb complessità ln(n)
void fixInsert(NodePtr z, NodePtr *radicec){
		NodePtr u;
		while (z->p->colore == 1) {
			if (z->p == z->p->p->dx) {
				u = z->p->p->sx; 
				if (u->colore == 1) {
					u->colore = 0;
					z->p->colore = 0;
					z->p->p->colore = 1;
					z = z->p->p;
				} else {
					if (z == z->p->sx) {
						z = z->p;
						rightRotate(z, radicec);
					}
					z->p->colore = 0;
					z->p->p->colore = 1;
					leftRotate(z->p->p, radicec);
				}
			} else {
				u = z->p->p->dx; 
				if (u->colore == 1) {					
					u->colore = 0;
					z->p->colore = 0;
					z->p->p->colore = 1;
					z = z->p->p;	
				} else {
					if (z == z->p->dx) {					
						z = z->p;
						leftRotate(z, radicec);
					}					
					z->p->colore = 0;
					z->p->p->colore = 1;
					rightRotate(z->p->p, radicec);
				}
			}
			if (z == *(radicec)) {
				break;
			}
		}
		(*(radicec))->colore = 0;
	}

//INSERIMENTO DI UN NUOVO NODO IN RB E SE è VALIDO ANCHE NELLA LISTA (VEDA INSERIMENTO DURANTE LA PARTITA)
//non considerando il fix_insert complessità costante con fix_insert teta di ln(n)
NodePtr insert(char *stringa, NodePtr *radicec, int validazione, Root *root, int conf) {
        NodePtr node = malloc(sizeof(elemento)); //creo il posto per il nodo in memoria
        char *st = malloc(sizeof(char)*k); //creo il posto per la parola nel nodo
        scrittura(stringa,st); //trasferisco la parola di puntatore

        node->str = st;
        node->sx = TNULL;
		node->dx = TNULL;
		node->colore = 1; // nuovo nodo di rosso
        node->valida = validazione; //così lo tengo traccia per dopo
        //0 di default se parte da zero il gioco 
        
        node->p = NULL;

        NodePtr y = NULL;
		NodePtr x = *(radicec);
        NodePtr register ult_valido = NULL;

		while (x != TNULL) {

            if(x->valida) {
                ult_valido = x; //vado a vedere ultimo valido così da inserire la parola in maniera corretta nella lista
            }

			y = x;

            if (posizione(node->str, x->str) < 0) {
				x = x->sx;
			} else {
				x = x->dx;
			}
		}

		node->p = y;

		if (y == NULL) {
			*(radicec) = node;
        } else if (posizione(node->str, y->str) < 0) {
			y->sx = node;
            //QUI VUOL DIRE CHE STA A SX
            if(validazione && conf){

                if(ult_valido != NULL){
                    node->next = ult_valido->next;

                    if(ult_valido->next != NULL) {
                        node->prev = ult_valido->next->prev;
                        ult_valido->next->prev = node;
                        ult_valido->next = node;
                    }
                    if(ult_valido->next == NULL) {
                        node->prev = ult_valido->prev;
                        ult_valido->prev->next = node;
                        root->fine_lista = node;
                    }
                }else{
                    //insrisco in testa
                    node->next = root->radice_lista;
                    if(root->radice_lista!=NULL) root->radice_lista->prev = node;
                    node->prev = NULL;
                    if(root->radice_lista==NULL) root->fine_lista = node;
                    root->radice_lista = node;
                }

            }

		} else {
			y->dx = node;
            //QUI VUOL DIRE CHE STA A DX
            if(validazione && conf){
                if(ult_valido != NULL){
                   
                    node->next = ult_valido;
                    node->prev = ult_valido->prev;
                
                    if(ult_valido->prev == NULL){
                        //in testa alla lista
                        root->radice_lista = node;
                        root->fine_lista = ult_valido;
                    }
                    else{
                        ult_valido->prev->next = node;
                        ult_valido->prev = node;
                    }
                }else{
                    //insrisco in testa
                    
                    node->next = root->radice_lista;
                    node->prev = NULL;

                    if(root->radice_lista!=NULL) {root->radice_lista->prev = node;}

                    if(root->radice_lista==NULL) root->fine_lista = node;

                    root->radice_lista = node;

                }
            } 
		}
		if (node->p == NULL){
			node->colore = 0;
			return node;
		}

		if (node->p->p == NULL) {
			return node;
        }
		fixInsert(node, radicec);
    return node;
}

//funzione che conta le parole con i requisiti giusti visti dal confronto
//stampo le parole filtrate anche qui devo comunque riverificare le vecchie valide perchè la nuova parole potrebbe cambiare i requisiti
//complessità teta(n*k^2) con n che diminuisce sempre di più perchè vengono filtrate fino a 1
//fa anche il conteggio
void stampa_lista_filtrato(NodePtr l,char *ver, char *rif,int i){
    while(l!=NULL) {
        if(validazione(l->str,ver, rif)){
            l->valida=1;
            if(i == 0) { cont_buone ++; //printf("%s\n", x->str);
            }
            else {
                printf("%s\n", l->str);
            }
        }else{
            l->valida=0;
            cancella(&lista_prova, l);
        }

        l = l->prev;
    }
}

//avendo letto una sola parola devo leggere tutte le parole e vedere se sono valide se sì inserisco nella lista
//complessità teta(n*k^2) con n totale delle parole inserite all'inizio
void conto_ordinata_filtrato(NodePtr x,char *ver, char *rif,int i){
    if(x!=TNULL){  
        conto_ordinata_filtrato(x->sx,ver,rif,i);
        if(validazione(x->str,ver, rif)){
            x->valida = 1; 
            if(i == 0) {
                cont_buone ++;
            }
            else {
                printf("%s\n", x->str);
            }
            //ricordo che vado a leggere le parole in ordine crescente e nella lista le metto in ordine decrescente così comeplssità costante
            //nell'inerimento e costante per leggere dalla fine visto che ho creato un puntatore alla fine
            inserisci_lista(&lista_prova,x);
            
        }else{
            x->valida=0;
        }
        conto_ordinata_filtrato(x->dx,ver,rif,i);
    }
}

//stampo tutte le parole di RB
//complessità teta(n) con n numero di parole ammissibili
void inOrder(NodePtr node) {
		if (node != TNULL) {
			inOrder(node->sx);
			printf("%s\n", node->str);
			inOrder(node->dx);
		} 
}

//verifico che la parola letta durante la partita faccia parte della parole ammissibili
//complessità teta(n*k) caso pesso se non è presente
void inOrder_controllo(NodePtr node, char* parola) {
		if (node != TNULL && trovata == 0) {
			inOrder_controllo(node->sx, parola);
            if(uguale(node->str,parola)) trovata = 1;
			inOrder_controllo(node->dx, parola);
		} 
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
    printf("%s\n", out); //stampo la stringa creata

}

int main(void){
    char c[4];
    if(fgets(c,4,stdin)!=NULL){}
    k = atoi(&c[0]); //leggo di quanto sono lunghe le parole

    char stringa[k]; //array di supporto per le parole lette
    char rif[k]; //memorizzo la parola di riferimento
    rif[0] = '&';
    int nuova = 0; //mi identifica inizio di una partita
    int inserimento = 1; //mi identifica se devo fare un inserimento di nuove patole
    int conteggio = 0;  //mi dice con quante parole devo confrontare la massimo 
    int primo_inserimento = 0; //letto la prima parola da confrontare
    int confronto_fatto = 0; //il confronto è stato fatto
    char ver[k+1]; ver[k] = '\0'; //stringa per le posizioni obbligatorie apprese


    init_rb(); //inizializzo RB
    init(ver); //preparo il dizionario

    while(fgets(stringa,5*k,stdin)!=NULL){ //uso fgets più veloce di fscanf
        if(stringa[0] == '+'){ //caso comandi
            if(stringa[1] == 'n'){
                //INIZIO DI UNA NUOVA PARTITA
                //svuoto la lista proveniente dalla vecchia partita
                if(lista_prova.radice_lista!=NULL || lista_prova.fine_lista != NULL) init_lista(&lista_prova);

                lista_prova.radice_lista=NULL;
                lista_prova.fine_lista=NULL;

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
                if(primo_inserimento && confronto_fatto) {
                    //confronto fatto ma non ancora creato la lista caso in cui confronto la prima parola della nuova partita con la parola di riferimento
                    conto_ordinata_filtrato(radice,ver,rif,1);primo_inserimento = 0;}
                else if(confronto_fatto){
                    //confronto fatto con lista già creata
                    stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,1);
                }else{
                    //SE NON HO ANCORA FATTO UN CONFRONTO STAMPO DIRETTAMENTE TUTTE LE PAROLE DAL RB
                    inOrder(radice);
                }

            }
        }else if(nuova){
            if(inserimento) {
                //inserimento dirante la partita allora conosco già alcuni requisiti delle parole da filtrate
                //mentre inserisco in RB inscerisco anche in lista se risulta essere valida
                insert(stringa, &radice, validazione(stringa,ver, rif), &lista_prova, confronto_fatto);

            }else{

                trovata = 0;
                inOrder_controllo(radice, stringa); //controllo la presenza nelle parole ammissibili
                if(uguale(rif,stringa)){
                        printf("ok\n"); //uguale 
                        nuova = 0; //FINSICE LA PARTITA
                        conteggio = 0; 
                        primo_inserimento = 1;
                }else if(!trovata){
                    printf("not_exists\n"); //non trovata
                }else if(conteggio != 1 ){ 
                        
                        char conf[k+1]; //creo array di supporto per il confronto in cui mettere elemento fuori 
                        confronto(rif,stringa, conf,ver);
                        confronto_fatto = 1;
                        conteggio = conteggio - 1; //aggiorno il conteggio
                        cont_buone = 0;

                        if(primo_inserimento) {
                            //confronto fatto ma non ancora creato la lista caso in cui confronto la prima parola della nuova partita con la parola di riferimento
                            conto_ordinata_filtrato(radice,ver,rif,0);primo_inserimento = 0;}
                            //conto_ordinata_filtrato(lista.radice,&lista_filtrata,ver,0);primo_inserimento = 0;}
                        else {
                             //confronto fatto con lista già creata
                            stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,0);
                        }
                        printf("%d\n",cont_buone);

                }else{
                    char conf[k+1];
                    confronto(rif,stringa, conf,ver);
                    cont_buone = 0;
                    //conto_ordinata(radice,ver,rif,0);
                    stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,0);
                    printf("%d\nko\n",cont_buone);
                    nuova = 0; //FINSICE LA PARTITA
                    conteggio = 0;
                    primo_inserimento = 1;
                }
                
            }
        }else if(inserimento) {
            //unserimento prima di ogni partita o alla fine qui imposto di default validazione a 0 perchè non conosco la patola di riferimento
            insert(stringa, &radice,0, &lista_prova, confronto_fatto);
        }
    }

    return 0;
}