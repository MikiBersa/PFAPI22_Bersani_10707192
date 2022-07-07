#include <stdio.h>
#include <stdlib.h>
//#define Debug 
#define DIZ 123

int k;
int cont_buone = 0;


//FACCIO UN ALBERO RB
typedef struct el{
    char *str;
    int colore; //0 è nero 1 è rosso
    int valida; //così so che è valida
    struct el *sx;
    struct el *dx;
    struct el *p;
    struct el *next; //per la lista interna o si fa anche qui un BST
    struct el *prev; //per la lista interna filtrate
} elemento;

typedef struct{
    int ex;
    char *no;
    int esatto;
    int min;
    int letto;
}filtro;

typedef elemento *NodePtr;

typedef struct{
    NodePtr radice_lista;
    NodePtr fine_lista;
}Root;

filtro diz[123];

NodePtr radice;
Root lista_prova;
NodePtr TNULL;

void init_rb(){
    TNULL = malloc(sizeof(elemento));
    TNULL->colore=0;
    TNULL->sx=NULL;
    TNULL->dx=NULL;
    radice = TNULL;
}

void init(char *ver){
    
    for(int i = 0; i<DIZ;i++){
        diz[i].no = malloc(sizeof(char)*k);
    }
}

void pulisci(char *ver){
    #ifdef Debug
        printf("In pulisci\n");
    #endif
    for(int i = 0; i<DIZ;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        #ifdef Debug
            printf("DAI valore %d\n", i);
        #endif
        for(int j = 0; j<k;j++){
            #ifdef Debug
                printf("Ci siamo %d\n", j);
            #endif
            diz[i].no[j] = '.';
            #ifdef Debug
                printf("Non problem 1\n");
            #endif
            ver[j]='.';
        }
        #ifdef Debug
            printf("Non problem\n");
        #endif
    }
}

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

int validazione(char *conf, char * ver, char * rif){
    int conteggio = 0;
    for(int i = 0;i<k;i++){
        int register num = (int) conf[i];
        if(conf[i]!=ver[i] && ver[i]!='.') return 0; //vuol dire che c'è
        else if(diz[num].ex==0) return 0; //confornto con le posizion obbligate
        else if(diz[num].ex==1){ //non c'è 
            if(diz[num].no[i]==conf[i]) return 0; //posizione sbagliata
            //if(diz[posizione_diz(conf[i])].per[i]!='.' && conf[i]!=diz[posizione_diz(conf[i])].per[i]) return 0; //non è quella obbligatoria
            conteggio = 0;
            for(int j = 0;j<k;j++) {
                if(conf[j]==conf[i]) //ci sono altre lettere nella parola come quella
                    conteggio ++;
            }

            if(diz[num].esatto!=0 && diz[num].esatto != conteggio) return 0;
            else if(diz[num].esatto==0){
            if(diz[num].min!=0 && conteggio <  diz[num].min) return 0;
            }
        }

    
    }
    //conosco la parola di riferimento
    //ANALISI DELLE PAROLE CHE NON HA QUELLA PAROLA MA CHE ALTRE HANNO
    int conto = 0;
    for(int l = 0; l<k;l++){
        int register num = (int) rif[l];
        if(diz[num].ex==1){ 
            //char car = inv_posizione(l);
            conto = 0;
            //strstr(haystack, needle);
            //if(!presente(car,conf,0))  return 0; //proprio non è presente
            //if(strstr(conf,&car) == NULL)  return 0;
            if(!presente(rif[l],conf,&conto))  return 0;
            else{
                if(diz[num].esatto != 0) {
                    if(conto!=diz[num].esatto) return 0;
                }else{
                    if(diz[num].min != 0 && conto <diz[num].min) return 0; //non è del numero minimo
                }
            }
            //printf("Ecco\n");
        }
    }

    return 1;
}

void filtrato(char * str1, char * out, char * ver){
    //int caret[k];
    //printf("CIAO\n");;
    int register num;
    //int poss = 0;
    for(int i = 0; i<k;i++){
        num = (int) str1[i];

        if(!diz[num].letto && diz[num].ex!=0){
            //printf("Dentro a letto\n");
            int pos = 0;
            int max = 0;
            int sl = 0;
            diz[num].letto = 1;

            for(int j = i+1; j<k;j++){
                if(str1[j]==str1[i]) {

                    if(out[j]=='/') {
                        sl = 1; //mi dice che ho dei valori esatti
                        diz[num].no[j]=str1[j];
                    }else{
                        pos = 1; //mi dice che escludo il non appartenere
                        max++;
                        if(out[j]=='+') ver[j] = str1[j]; 
                        else diz[num].no[j]=str1[j];
                    
                    }
                }
            }
            if(out[i]=='+') {
                ver[i] = str1[i];
                
                diz[num].ex = 1;
                if(sl && pos){
                    diz[num].esatto = max+1;
                }

                if((max+1) > diz[num].min) diz[num].min = max+1;
            }
            else if(out[i]=='/'){
                if(pos) {
                    diz[num].no[i]=str1[i];
                    diz[num].esatto = max;
                    diz[num].ex = 1;
                }else diz[num].ex = 0;
            }else if(out[i]=='|'){
                diz[num].ex = 1;
                if(!sl) {
                    if((max+1) > diz[num].min) diz[num].min = max+1; //aggiorno dopo altre letture
                }
                else {diz[num].esatto = max+1;}
                diz[num].no[i]=str1[i];
            }
        }
    }
    //poss++;
    //printf("POSIZIONE %d\n", pos-1);
    //for(int i = 0; i<poss-1;i++) {diz[caret[i]].letto = 0;}
    //printf("Ci arrivo\n");
    for(int i = 0; i<k;i++) {diz[(int) str1[i]].letto = 0;}
    //for(int i = 0; i<64;i++) {diz[i].letto = 0;}
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


//LISTA
void inserisci_lista(Root *root, NodePtr x){
    x->next = root->radice_lista;
    if(root->radice_lista!=NULL) root->radice_lista->prev = x;
    if(root->radice_lista==NULL) root->fine_lista = x;
    root->radice_lista = x;
    x->prev = NULL;
}

//costante eliminazione
void cancella(Root *root, NodePtr x){
    if(x->prev!=NULL) x->prev->next = x->next;
    else (*root).radice_lista = x->next;
    if(x->next != NULL) x->next->prev = x->prev;
    else (*root).fine_lista = x->prev;
    //printf("CANCELLATO %s\n", x->str);
}

void init_lista(Root *l){
    #ifdef Debug
        printf("PULIZIA DA VERIFICARE\n");
    #endif
    NodePtr p = l->radice_lista;
    //PULIZIA DEI NODI
    while(p!=NULL) {
        p->valida = 0;
        cancella(&lista_prova, p); 
        p = p->next;
    }
}

//TEAT DI N
void stampa_lista(Root l){
    elemento *p = l.fine_lista;
    while(p!=NULL) {
        printf("%s\n", p->str);
        p = p->prev;
    }
}

//RB DEL ALBERO NORMALE
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

void fixInsert(NodePtr z, NodePtr *radicec){
		NodePtr u;
		while (z->p->colore == 1) {
			if (z->p == z->p->p->dx) {
				u = z->p->p->sx; // uncle
				if (u->colore == 1) {
					// case 3.1
					u->colore = 0;
					z->p->colore = 0;
					z->p->p->colore = 1;
					z = z->p->p;
				} else {
					if (z == z->p->sx) {
						// case 3.2.2
						z = z->p;
						rightRotate(z, radicec);
					}
					// case 3.2.1
					z->p->colore = 0;
					z->p->p->colore = 1;
					leftRotate(z->p->p, radicec);
				}
			} else {
				u = z->p->p->dx; // uncle

				if (u->colore == 1) {
					// mirror case 3.1
					u->colore = 0;
					z->p->colore = 0;
					z->p->p->colore = 1;
					z = z->p->p;	
				} else {
					if (z == z->p->dx) {
						// mirror case 3.2.2
						z = z->p;
						leftRotate(z, radicec);
					}
					// mirror case 3.2.1
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


NodePtr insert(char *stringa, NodePtr *radicec, int validazione, Root *root, int conf) {
        //VEDERE COME FARE CON VALIDAZIONE E COLLEGARLO ALLA LISTA
        //PER EVITARE DI FARLA SCORRERE SEMRPE
        NodePtr node = malloc(sizeof(elemento));
        char *st = malloc(sizeof(char)*k);
        //strncpy(st,stringa,k);
        scrittura(stringa,st);
        //memcpy(st,stringa,k);

        node->str = st;
        node->sx = TNULL;
		node->dx = TNULL;
		node->colore = 1; // new node must be red
        node->valida = validazione; //così lo tengo traccia per dopo
        //0 di default se parte da zero il gioco 
        
        node->p = NULL;

        NodePtr y = NULL;
		NodePtr x = *(radicec);
        NodePtr register ult_valido = NULL;

		while (x != TNULL) {

            if(x->valida) {
                ult_valido = x; //che risulta essere poi y
                
                #ifdef Debug
                    printf("Valido stringa %s\n", ult_valido->str);
                #endif
            }
            //inserimento di prima o dopo dipende dalla posizione rispetto a y

			y = x;
            
			//if (strncmp(node->str, x->str,k) < 0) {
            if (posizione(node->str, x->str) < 0) {
            //if (memcmp(node->str, x->str,k) < 0) {
				x = x->sx;
			} else {
				x = x->dx;
			}
		}
        
        #ifdef Debug
            if(ult_valido != NULL) printf("ultimo %s ciao\n",ult_valido->str);
        #endif
		// y is parent of x
		node->p = y;
        #ifdef Debug
            if(ult_valido != NULL) printf("ultimo %s ciao2\n",ult_valido->str);
        #endif
		if (y == NULL) {
            #ifdef Debug
                printf("Dentro\n");
            #endif
			*(radicec) = node;
		//} else if (strncmp(node->str, y->str,k) < 0) {
        } else if (posizione(node->str, y->str) < 0) {
        //} else if (memcmp(node->str, y->str,k) < 0) {
			y->sx = node;
            //QUI VUOL DIRE CHE STA A SX
            #ifdef Debug
                if(ult_valido != NULL) printf("ultimo %s ciao3\n",ult_valido->str);
            #endif
            #ifdef Debug
                printf("SX entro %s\n", y->str);
            #endif
            if(validazione && conf){
                #ifdef Debug
                    printf("In validazione\n");
                #endif
                #ifdef Debug
                    if(ult_valido != NULL) printf("ultimo %s ciao4\n",ult_valido->str);
                #endif
                //inseirmento in lista a dx (siccome lista è ordinata al contrario)
                if(ult_valido != NULL){
                    #ifdef Debug
                        if(ult_valido != NULL) printf("ultimo %s ciao5\n",ult_valido->str);
                    #endif
                    #ifdef Debug
                        printf("ultimo %s\n",ult_valido->str);
                        if(ult_valido->next != NULL) {
                            printf("Diverso DA NULL\n");
                            printf("Prova %s\n",ult_valido->next->str);
                        }
                        //printf("ULTIMO\n");
                        //printf("ultimo %s",ult_valido->str);
                    #endif
                    node->next = ult_valido->next;
                    #ifdef Debug
                    printf("Ci siamo\n");
                    #endif
                    
                    if(ult_valido->next != NULL) {
                        node->prev = ult_valido->next->prev;
                        ult_valido->next->prev = node;
                        ult_valido->next = node;
                    }
                    #ifdef Debug
                    printf("Eccodi\n");
                    #endif
                    if(ult_valido->next == NULL) {
                        node->prev = ult_valido->prev;
                        ult_valido->prev->next = node;
                        root->fine_lista = node;
                    }
                }else{
                    //insrisco in testa
                    #ifdef Debug
                    printf("ultimo NULL\n");
                    #endif
                    node->next = root->radice_lista;
                    if(root->radice_lista!=NULL) root->radice_lista->prev = node;
                    node->prev = NULL;
                    if(root->radice_lista==NULL) root->fine_lista = node;
                    root->radice_lista = node;
                }
                #ifdef Debug
                printf("ESCE\n");
                #endif
            }

		} else {
			y->dx = node;
            //QUI VUOL DIRE CHE STA A DX
            #ifdef Debug
            printf("DX entro\n");
            #endif
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
            } //inseirmento in lista
		}
		// if new node is a root node, simply return
		if (node->p == NULL){
			node->colore = 0;
			return node;
		}

		// if the grandparent is null, simply return
		if (node->p->p == NULL) {
			return node;
        }
		// Fix the tree
		fixInsert(node, radicec);
    return node;
}

void conto_ordinata(NodePtr x,char *ver, char *rif,int i){
    if(x!=TNULL){    
        conto_ordinata(x->sx,ver,rif,i);
        if(validazione(x->str,ver,rif)){
            if(i == 0) { x->valida = 0; cont_buone ++; //printf("%s\n", x->str);
            }
            else printf("%s\n", x->str);
        }else{
            //elimino nell'albero
            //deleteNodeHelper(&root_filtrato,x);
            x->valida = 0;
            cancella(&lista_prova, x);
            //printf("Eliminazione: %s\n", x->str);

        }
        conto_ordinata(x->dx,ver,rif,i);
    }
}

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


void conto_ordinata_filtrato(NodePtr x,char *ver, char *rif,int i){
    if(x!=TNULL){  
        //printf("DENTRO\n");  
        conto_ordinata_filtrato(x->sx,ver,rif,i);
        if(validazione(x->str,ver, rif)){
            //printf("Dentro a validazione\n");
            x->valida = 1; 
            if(i == 0) {
                cont_buone ++;
            }
            else {
                printf("%s\n", x->str);
            }
                //inserimento nel nuovo bst
            //insert(x->str, &root_filtrato); //occhio che me li inserisce già ordinati -> facendo così riduco solo il teta di n come numero
            inserisci_lista(&lista_prova,x);
            //dovrei trasformare anche questo in RB
            
        }else{
            x->valida=0;
        }
        conto_ordinata_filtrato(x->dx,ver,rif,i);
    }
}

void inOrder(NodePtr node) {
		if (node != TNULL) {
			inOrder(node->sx);
			printf("%s\n", node->str);
			inOrder(node->dx);
		} 
}

int trovata = 0;

void inOrder_controllo(NodePtr node, char* parola) {
		if (node != TNULL && trovata == 0) {
			inOrder_controllo(node->sx, parola);
			//if(!strncmp(node->str,parola,k)) trovata = 1;
            if(uguale(node->str,parola)) trovata = 1;
            //if(!memcmp(node->str,parola,5)) trovata = 1;
			inOrder_controllo(node->dx, parola);
		} 
}

//PROVARE A FARE UN UNICO CICLO per ora ci sono 5 cicli
void confronto(char* str2,char* str1, char *out){
    //str2 è il riferimento
    int cont = 0;
    int giu = 0;
    char c;

    for(int i = 0; i<=k-1;i++){
        if(str1[i] == str2[i]) out[i] = '+';
        else{
            cont = 0;
            giu = 0;
            c = str1[i];
            
            for(int j = 0; j<=k-1;j++){
                if(str1[i] == str2[j]) {
                    cont ++;
                    if(i!=j) out[i] = '|'; 
                    if(str1[j] == str2[j] && out[j] != '+') out[j] = '+';
                    if(str1[j] == c && out[j] == '+') giu ++;
                }
            }
            if(out[i] != '|') out[i] = '/';
            else{
                cont = cont - giu;
                giu = 0;
                 for(int l = i-1; l>=0; l--){
                    if(str1[l] == c && out[l] == '|') giu ++;
                }
                if(giu >= cont || cont == 0) out[i] = '/';
            }


        }
    }
    
    out[k] ='\0'; // VERIFICARE CON IL DIFF VEDERE COSA VUOLE
   
    //printf("%s %s\n",str1, out);
   
    printf("%s\n", out);

}

int main(void){
    //elemento * lista = NULL;
    char c[4];
    //if(scanf("%d", &k) != EOF){}
    //printf("INIZIO\n");
    if(fgets(c,4,stdin)!=NULL){}
    k = atoi(&c[0]);
   //printf("VALORE: %d\n", k);
    char stringa[k];
    char rif[k];
    rif[0] = '&';
    int nuova = 0;
    int inserimento = 1;
    int conteggio = 0;
    int primo_inserimento = 0;
    int confronto_fatto = 0;
    char ver[k+1]; ver[k] = '\0';


    init_rb();
    init(ver); //preparo il dizionario

    while(fgets(stringa,10*k,stdin)!=NULL){
        //if(scanf("%s", stringa)!=EOF){}
        #ifdef Debug
            printf("Letto %s",stringa);
        #endif
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                //CONTROLLARE SE è VUOTA SE NO IMPOSTARE VALID = 0
                if(lista_prova.radice_lista!=NULL || lista_prova.fine_lista != NULL) init_lista(&lista_prova);

                lista_prova.radice_lista=NULL;
                lista_prova.fine_lista=NULL;
                nuova = 1;
                inserimento = 0;
                primo_inserimento = 1;
                //printf("INSERIMENTO PAROLE FINE\n");
                #ifdef Debug
                    printf("Dentro\n");
                    printf("INIZIO_PARTITA in +\n");
                #endif
                pulisci(ver); //riazzero il dizionario
                //inserisco i nuovi elementi 
                #ifdef Debug
                    printf("PULITO\n");
                #endif
                if(fgets(stringa,10*k,stdin)!=NULL){}
                scrittura(stringa,rif);
                //strncpy(rif,stringa,k);
                //memcpy(rif,stringa,k);
                if(fgets(stringa,10*k,stdin)!=NULL){}
                conteggio = atoi(&stringa[0]);
                confronto_fatto = 0;
                //printf("CONTEGGIO %d\n", conteggio);
            }
            else if(stringa[1] == 'i'){
                if(stringa[11] == 'i') {
                    inserimento = 1;
                    //printf("INSERIMENTO PAROLE INZIO\n");
                }
                else if(stringa[11] == 'f'){
                    inserimento = 0;
                    //printf("INSERIMENTO PAROLE FINE\n");
                }
            }
            else if(stringa[1] == 's' && stringa[2] == 't'){
                //scrivi();
                //conto_ordinata(lista,ver,1);
                if(primo_inserimento && confronto_fatto) {
                    //printf("Prima volta\n");
                    conto_ordinata_filtrato(radice,ver,rif,1);primo_inserimento = 0;}
                else if(confronto_fatto){
                    //printf("NOn prima volta\n");
                    //conto_ordinata(root_filtrato,ver,1); 
                    stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,1);
                }else{
                    inOrder(radice);
                }

            }else if(stringa[1] == '2'){
                
                    printf("Stampo albero\n");
                    inOrder(radice);
                    //printf("Stampo lista\n");
                    //stampa_lista(lista_prova);
                
            }
        }else if(nuova){
            if(inserimento) {
                #ifdef Debug
                    printf("Inserimento in mezzo al gioco\n");
                #endif

                insert(stringa, &radice, validazione(stringa,ver, rif), &lista_prova, confronto_fatto);

            }else{

                trovata = 0;
                inOrder_controllo(radice, stringa);
                //if(!strncmp(rif,stringa,k)){
                if(uguale(rif,stringa)){
                //if(!memcmp(rif,stringa,k)){
                        printf("ok\n");
                        //fputs("ok\n",stdout);
                        nuova = 0; //FINSICE LA PARTITA
                        conteggio = 0; //DA VERIFICARE
                        //rif[0] = '&';
                        //lista_filtrata = NULL; //pulisco il bst 
                        primo_inserimento = 1;
                }else if(!trovata){
                    printf("not_exists\n");
                }else if(conteggio != 1 ){ 
                    //printf("Stringa letta in : %s\n", stringa);
                    //if(controllo(lista,stringa)){
                        
                        char conf[k+1];
                        confronto(rif,stringa, conf);
                        filtrato(stringa,conf, ver);

                        confronto_fatto = 1;
                        //printf("CONTEGGIO prima: %D\n", conteggio);
                        conteggio = conteggio - 1;
                        //printf("CONTEGGIO dopo: %D\n", conteggio);
                        //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                        cont_buone = 0;

                        if(primo_inserimento) {
                            //printf("Prima volta\n");
                            conto_ordinata_filtrato(radice,ver,rif,0);primo_inserimento = 0;}
                            //conto_ordinata_filtrato(lista.radice,&lista_filtrata,ver,0);primo_inserimento = 0;}
                        else {
                            //printf("NOn prima volta\n");
                            //conto_ordinata(root_filtrato,ver,0); 
                            stampa_lista_filtrato(lista_prova.fine_lista,ver,rif,0);
                            //conto_ordinata(lista_filtrata,&lista_filtrata,ver,0); //faccio il conteggio sul nuovo bst
                        }
                        printf("%d\n",cont_buone);

                }else{
                    //printf("CONTEGGIO in ko: %D\n", conteggio);
                    char conf[k+1];
                    confronto(rif,stringa, conf);
                    filtrato(stringa,conf, ver);
                    //filtrato(stringa,confronto(rif,stringa), ver);
                    //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                    cont_buone = 0;
                    conto_ordinata(radice,ver,rif,0);
                    printf("%d\nko\n",cont_buone);
                    nuova = 0; //FINSICE LA PARTITA
                    conteggio = 0;
                    //lista_filtrata = NULL;
                    primo_inserimento = 1;
                    //rif[0] = '&';
                }
                
            }
        }else if(inserimento) {
            //printf("Inerito %s prima dell'ins\n", stringa);
            insert(stringa, &radice,0, &lista_prova, confronto_fatto);
            //printf("Inserito nuove stringhe ciao\n");
        }
    }

    //scrittura_ordinata(lista);
    
    return 0;
}