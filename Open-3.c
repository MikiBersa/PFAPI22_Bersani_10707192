#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#define debug 

int k;
int cont_buone = 0;

//FACCIO UN ALBERO RB
typedef struct el{
    char *str;
    int colore; //0 è nero 1 è rosso
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

filtro diz[64];

NodePtr lista;
NodePtr radice;
NodePtr TNULL;

void init_rb(){
    TNULL = malloc(sizeof(elemento));
    TNULL->colore=0;
    TNULL->sx=NULL;
    TNULL->dx=NULL;
    lista = NULL;
    radice = TNULL;
}

void init(char *ver){
    
    for(int i = 0; i<64;i++){
        diz[i].esatto=0;
        diz[i].letto=0;
        diz[i].min=0;
        diz[i].ex = -1;
        diz[i].no = malloc(sizeof(char)*k);
        for(int j = 0; j<k;j++){
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
        int register num = posizione_diz(conf[i]);
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

    //ANALISI DELLE PAROLE CHE NON HA QUELLA PAROLA MA CHE ALTRE HANNO
    for(int l = 0; l<64;l++){
        if(diz[l].ex==1){ 
            char car = inv_posizione(l);
            //strstr(haystack, needle);
            //if(!presente(car,conf,0))  return 0; //proprio non è presente
            //if(strstr(conf,&car) == NULL)  return 0;
            if(!presente(car,conf,0))  return 0;
            else{
                if(diz[l].esatto != 0 && presente(car,conf,1)!=diz[l].esatto) return 0;
                if(diz[l].min != 0 && presente(car,conf,1)<diz[l].min) return 0; //non è del numero minimo
            }
        }
    }

    return 1;
}

void filtrato(char * str1, char * out, char * ver){
    for(int i = 0; i<k;i++){
        int register num = posizione_diz(str1[i]);
        if(!diz[num].letto && diz[num].ex!=0){
            //printf("Dentro a letto\n");
            int pos = 0;
            int max = 0;
            int sl = 0;
            diz[num].letto = 1;
            
            for(int j = i+1; j<k;j++){
                if(str1[j]==str1[i]) {
                    if((out[j]!='/')) { 
                        pos = 1; //mi dice che escludo il non appartenere
                        max++;
                    }
                    if(out[j]=='|') diz[num].no[j]=str1[j];
                    if(out[j]=='/') {
                        sl = 1; //mi dice che ho dei valori esatti
                        diz[num].no[j]=str1[j];
                    }
                    if(out[j]=='+') {
                        ver[j] = str1[j];
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

    for(int i = 0; i<k;i++) {diz[posizione_diz(str1[i])].letto = 0;}
    //for(int i = 0; i<64;i++) {diz[i].letto = 0;}
}
/*
int posizione(char* c1, char *c2){ //c2 la testa
    for(int i = 0; i<k;i++){
        if(c1[i] > c2[i]) return 1; //destra
        else if(c1[i] < c2[i]) return -1; //sinistra
    }
    return 0;
}
*/
/*
void scrittura(char* c1, char *c2){ //da c1 a c2
    for(int i = 0; i<k;i++){
        c2[i] = c1[i];
    }
}
*/

//LISTA
void inserisci_lista(NodePtr *root, NodePtr x){
    NodePtr puntCorrente, puntPrecedente;
    puntPrecedente = NULL;
    puntCorrente = *root;

    while(puntCorrente != NULL && strncmp(puntCorrente->str, x->str,k) < 0){
        puntPrecedente = puntCorrente;
        puntCorrente = puntPrecedente->next;
    }

    x->next = puntCorrente;
    if(*root == NULL) x->prev = NULL;
    else x->prev = puntPrecedente;

    if(puntPrecedente != NULL) puntPrecedente->next = x;
    else *root = x;
}

void cancella(NodePtr *root, NodePtr x){
    if(x->prev!=NULL) x->prev->next = x->next;
    else (*root) = x->next;
    if(x->next != NULL) x->next->prev = x->prev;
}

void stampa_lista(NodePtr l){
    while(l->next!=NULL) printf("%s", l->str);
    printf("%s", l->str);
}

//RB DEL ALBERO NORMALE
NodePtr maximum(NodePtr node) {
	while (node->dx != TNULL) {
		node = node->dx;
	}
	return node;
}

NodePtr minimum(NodePtr node) {
	while (node->sx != TNULL) {
		node = node->sx;
	}
	return node;
}

NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->dx != TNULL) {
			return minimum(x->dx);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->p;
		while (y != TNULL && x == y->dx) {
			x = y;
			y = y->p;
		}
		return y;
}

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

void fixInsert(NodePtr k, NodePtr *radicec){
		NodePtr u;
		while (k->p->colore == 1) {
			if (k->p == k->p->p->dx) {
				u = k->p->p->sx; // uncle
				if (u->colore == 1) {
					// case 3.1
					u->colore = 0;
					k->p->colore = 0;
					k->p->p->colore = 1;
					k = k->p->p;
				} else {
					if (k == k->p->sx) {
						// case 3.2.2
						k = k->p;
						rightRotate(k, radicec);
					}
					// case 3.2.1
					k->p->colore = 0;
					k->p->p->colore = 1;
					leftRotate(k->p->p, radicec);
				}
			} else {
				u = k->p->p->dx; // uncle

				if (u->colore == 1) {
					// mirror case 3.1
					u->colore = 0;
					k->p->colore = 0;
					k->p->p->colore = 1;
					k = k->p->p;	
				} else {
					if (k == k->p->dx) {
						// mirror case 3.2.2
						k = k->p;
						leftRotate(k, radicec);
					}
					// mirror case 3.2.1
					k->p->colore = 0;
					k->p->p->colore = 1;
					rightRotate(k->p->p, radicec);
				}
			}
			if (k == *(radicec)) {
				break;
			}
		}
		(*(radicec))->colore = 0;
	}


NodePtr insert(char *stringa, NodePtr *radicec) {

        NodePtr node = malloc(sizeof(elemento));
        char *st = malloc(sizeof(char)*k);
        strncpy(st,stringa,k);

        node->str = st;
        node->sx = TNULL;
		node->dx = TNULL;
		node->colore = 1; // new node must be red

        node->p = NULL;

        NodePtr y = NULL;
		NodePtr x = *(radicec);

		while (x != TNULL) {
			y = x;
			if (strncmp(node->str, x->str,k) < 0) {
				x = x->sx;
			} else {
				x = x->dx;
			}
		}
        //printf("Arrivato\n");
		// y is parent of x
		node->p = y;
		if (y == NULL) {
            //printf("Dentro\n");
			*(radicec) = node;
		} else if (strncmp(node->str, y->str,k) < 0) {
			y->sx = node;
		} else {
			y->dx = node;
		}

		// if new node is a root node, simply return
		if (node->p == NULL){
            //printf("Dentro 2\n");
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

/*
void fixDelete(NodePtr x, NodePtr *radice) {
		NodePtr s;
		while (x != *(radice) && x->colore == 0) {
			if (x == x->p->sx) {
				s = x->p->dx;
				if (s->colore == 1) {
					// case 3.1
					s->colore = 0;
					x->p->colore = 1;
					leftRotate(x->p, radice);
					s = x->p->dx;
				}

				if (s->sx->colore == 0 && s->dx->colore == 0) {
					// case 3.2
					s->colore = 1;
					x = x->p;
				} else {
					if (s->dx->colore == 0) {
						// case 3.3
						s->sx->colore = 0;
						s->colore = 1;
						rightRotate(s, radice);
						s = x->p->dx;
					} 

					// case 3.4
					s->colore = x->p->colore;
					x->p->colore = 0;
					s->dx->colore = 0;
					leftRotate(x->p, radice);
					x = *(radice);
				}
			} else {
				s = x->p->sx;
				if (s->colore == 1) {
					// case 3.1
					s->colore = 0;
					x->p->colore = 1;
					rightRotate(x->p, radice);
					s = x->p->sx;
				}

				if (s->dx->colore == 0 && s->dx->colore == 0) {
					// case 3.2
					s->colore = 1;
					x = x->p;
				} else {
					if (s->sx->colore == 0) {
						// case 3.3
						s->dx->colore = 0;
						s->colore = 1;
						leftRotate(s, radice);
						s = x->p->sx;
					} 

					// case 3.4
					s->colore = x->p->colore;
					x->p->colore = 0;
					s->sx->colore = 0;
					rightRotate(x->p, radice);
					x = *(radice);
				}
			} 
		}
		x->colore = 0;
	}

void rbTransplant(NodePtr u, NodePtr v, NodePtr *radice){
		if (u->p == NULL) {
			*(radice) = v;
		} else if (u == u->p->sx){
			u->p->sx = v;
		} else {
			u->p->dx = v;
		}
		v->p = u->p;
	}

void deleteNodeHelper(NodePtr* radice, NodePtr z) {
		// find the node containing key
        /*
		NodePtr x, y; 
		
        if(s->sx==TNULL || s->dx == TNULL){
            y = s;
        }else { 
            y = successor(s); 
        }
        if(y->sx!=TNULL) x = y->sx;
        else x = y->dx;

        x->p= y->p;
        
        if(y->p==TNULL) *(radice) = x;
        else if(y==y->p->sx) y->p->sx = x;
        else y->p->dx = x;
        if(y!=s) {
            strncpy(s->str,y->str,k);
        }
        free(s);
		if (y->colore == 0){
			fixDelete(x, radice);
		}
        */
       /*
       NodePtr x, y; 
       //NodePtr z = TNULL;
       y = z;
		int y_original_color = y->colore;
		if (z->sx == TNULL) {
			x = z->dx;
			rbTransplant(z, z->dx, radice);
		} else if (z->dx == TNULL) {
			x = z->sx;
			rbTransplant(z, z->sx, radice);
		} else {
			y = minimum(z->dx);
			y_original_color = y->colore;
			x = y->dx;
			if (y->p == z) {
				x->p = y;
			} else {
				rbTransplant(y, y->dx, radice);
				y->dx = z->dx;
				y->dx->p = y;
			}

			rbTransplant(z, y, radice);
			y->sx = z->sx;
			y->sx->p = y;
			y->colore = z->colore;
		}
		free(z);
		if (y_original_color == 0){
			fixDelete(x, radice);
		}
	}

*/
void conto_ordinata(NodePtr x,char *ver, int i){
    if(x!=TNULL){    
        conto_ordinata(x->sx,ver,i);
        if(validazione(x->str,ver)){
            if(i == 0) {cont_buone ++; //printf("%s\n", x->str);
            }
            else printf("%s\n", x->str);
        }else{
            //elimino nell'albero
            //deleteNodeHelper(&root_filtrato,x);
            cancella(&lista, x);
            //printf("Eliminazione: %s\n", x->str);

        }
        conto_ordinata(x->dx,ver,i);
    }
}

void stampa_lista_filtrato(NodePtr l,char *ver, int i){
    while(l!=NULL) {
        if(validazione(l->str,ver)){
            if(i == 0) {cont_buone ++; //printf("%s\n", x->str);
            }
            else printf("%s\n", l->str);
        }else{
            cancella(&lista, l);
        }
       
        l = l->next;
    }
}


void conto_ordinata_filtrato(NodePtr x,char *ver, int i){
    if(x!=TNULL){  
        //printf("DENTRO\n");  
        conto_ordinata_filtrato(x->sx,ver,i);
        if(validazione(x->str,ver)){
            //printf("Dentro a validazione\n");
            if(i == 0) {cont_buone ++;}
            else printf("%s\n", x->str);
                //inserimento nel nuovo bst
            //insert(x->str, &root_filtrato); //occhio che me li inserisce già ordinati -> facendo così riduco solo il teta di n come numero
            inserisci_lista(&lista, x);
            //dovrei trasformare anche questo in RB
            
        }
        conto_ordinata_filtrato(x->dx,ver,i);
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
			if(!strncmp(node->str,parola,k)) trovata = 1;
			inOrder_controllo(node->dx, parola);
		} 
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
   
    //printf("%s %s\n",str1, out);
   
    printf("%s\n", out);
   
    char * rit = malloc(sizeof(char)*k);
    //scrittura(out, rit);
    strncpy(rit,out,k);
    return rit;
}

int main(void){
    //elemento * lista = NULL;

    if(scanf("%d", &k) != EOF){}

    char stringa[k];
    char rif[k];
    rif[0] = '&';
    int nuova = 0;
    int inserimento = 1;
    int conteggio = 0;
    int primo_inserimento = 0;
    char ver[k+1]; ver[k] = '\0';

    init_rb();
    init(ver); //preparo il dizionario

    while(!feof(stdin)){
        if(scanf("%s", stringa)!=EOF){}
        //printf("Letto %s\n",stringa);
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                lista = NULL;
                nuova = 1;
                inserimento = 0;
                primo_inserimento = 1;
                //printf("INSERIMENTO PAROLE FINE\n");
                //printf("INIZIO_PARTITA in +\n");
                pulisci(ver); //riazzero il dizionario
                //inserisco i nuovi elementi 
                if(scanf("%s", stringa)!=EOF){}
                //scrittura(stringa,rif);
                strncpy(rif,stringa,k);
                if(scanf("%s", stringa)!=EOF){}
                conteggio = atoi(&stringa[0]);
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
                if(primo_inserimento) {
                    //printf("Prima volta\n");
                    conto_ordinata_filtrato(radice,ver,1);primo_inserimento = 0;}
                else {
                    //printf("NOn prima volta\n");
                    //conto_ordinata(root_filtrato,ver,1); 
                    stampa_lista_filtrato(lista,ver,1);
                }//faccio il conteggio sul nuovo bst
                //conto_ordinata(lista_filtrata,ver,1);
                //printf("S\n");
            }else if(stringa[1] == '2'){
                    inOrder(radice);
            }
        }else if(nuova){
            //insrriemnto durante la partita
            if(inserimento) {
                NodePtr prova = insert(stringa, &radice);
                //printf("Inserito nuove stringhe\n");
                if(validazione(stringa,ver)){
                    //se è gia valido lo metto li dentro
                    //printf("INSERISCO %s\n", stringa);
                    //insert(stringa, &root_filtrato);
                    inserisci_lista(&lista, prova);
                }
            }else{
                //Qui dovrebbe partire algoritmo 
                //printf("Stringhe da confrontare %s\n", stringa);
                //VEDERE SE APPARTIENE O NO ALLE PAROLE AMMISSBILI
                //printf("CONTEGGIO prima -1: %D\n", conteggio);
                //printf("Stringa letta in : %s\n", stringa);
                //if(uguale(rif,stringa)){
                //printf("Stampo le parole\n");
                //inOrder(radice);
                trovata = 0;
                inOrder_controllo(radice, stringa);
                if(!strncmp(rif,stringa,k)){
                        printf("ok\n");
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
                        char *ritorno;
                        ritorno = confronto(rif,stringa);
                        filtrato(stringa,ritorno, ver);
                        free(ritorno);
                        //printf("CONTEGGIO prima: %D\n", conteggio);
                        conteggio = conteggio - 1;
                        //printf("CONTEGGIO dopo: %D\n", conteggio);
                        //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                        cont_buone = 0;

                        if(primo_inserimento) {
                            //printf("Prima volta\n");
                            conto_ordinata_filtrato(radice,ver,0);primo_inserimento = 0;}
                            //conto_ordinata_filtrato(lista.radice,&lista_filtrata,ver,0);primo_inserimento = 0;}
                        else {
                            //printf("NOn prima volta\n");
                            //conto_ordinata(root_filtrato,ver,0); 
                            stampa_lista_filtrato(lista,ver,0);
                            //conto_ordinata(lista_filtrata,&lista_filtrata,ver,0); //faccio il conteggio sul nuovo bst
                        }
                        printf("%d\n",cont_buone);
                        #ifdef debug
                            conto_ordinata(lista,ver,1);
                            scrivi(ver);
                        #endif
                    //}else{
                        //printf("CONTEGGIO in NON exists: %D\n", conteggio);
                    //    printf("not_exists\n");
                    //}
                }else{
                    //printf("CONTEGGIO in ko: %D\n", conteggio);
                    char *ritorno;
                    ritorno = confronto(rif,stringa);
                    filtrato(stringa,ritorno, ver);
                    free(ritorno);
                    //SCRIVERE IL CONTEGGIO DELLE FILTRATE BUONE
                    cont_buone = 0;
                    conto_ordinata(radice,ver,0);
                    printf("%d\n",cont_buone);
                    
                    printf("ko\n");
                    #ifdef debug
                        conto_ordinata(lista_filtrata,ver,1);
                        scrivi(ver);
                    #endif
                    pulisci(ver);
                    nuova = 0; //FINSICE LA PARTITA
                    conteggio = 0;
                    //lista_filtrata = NULL;
                    primo_inserimento = 1;
                    //rif[0] = '&';
                }
                
            }
        }else if(inserimento) {
            //printf("Inerito %s prima dell'ins\n", stringa);
            insert(stringa, &radice);
            //printf("Inserito nuove stringhe\n");
        }
    }

    //scrittura_ordinata(lista);
    
    return 0;
}