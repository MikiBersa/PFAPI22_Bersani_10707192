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
    int colore_filt;
    struct el *sx;
    struct el *dx;
    struct el *p;
    struct el *parent; // pointer to the parent
	struct el *left; // pointer to left child
	struct el *right; // pointer to right child
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

NodePtr root_filtrato;
NodePtr radice;
NodePtr TNULL;

void init_rb(){
    TNULL = malloc(sizeof(elemento));
    TNULL->colore=0;
    TNULL->colore_filt=0;
    TNULL->left=NULL;
    TNULL->right=NULL;
    TNULL->sx=NULL;
    TNULL->dx=NULL;
    root_filtrato = TNULL;
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
        if(conf[i]!=ver[i] && ver[i]!='.') return 0; //vuol dire che c'è
        else if(diz[posizione_diz(conf[i])].ex==0) return 0; //confornto con le posizion obbligate
        else if(diz[posizione_diz(conf[i])].ex==1){ //non c'è 
            if(diz[posizione_diz(conf[i])].no[i]==conf[i]) return 0; //posizione sbagliata
            //if(diz[posizione_diz(conf[i])].per[i]!='.' && conf[i]!=diz[posizione_diz(conf[i])].per[i]) return 0; //non è quella obbligatoria
            conteggio = 0;
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
            //strstr(haystack, needle);
            //if(!presente(car,conf,0))  return 0; //proprio non è presente
            //if(strstr(conf,&car) == NULL)  return 0;
            if(!presente(car,conf,0))  return 0;
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
                        ver[j] = str1[j];
                    }
                }
            }
            if(out[i]=='+') {
                ver[i] = str1[i];
                
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

//RB DEL FILTRO
NodePtr maximum_filtrato(NodePtr node) {
	while (node->right != TNULL) {
		node = node->right;
	}
	return node;
}

NodePtr minimum_filtrato(NodePtr node) {
	while (node->left != TNULL) {
		node = node->left;
	}
	return node;
}

NodePtr successor_filtrato(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum_filtrato(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
}

void rightRotate_filtrato(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root_filtrato = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

void leftRotate_filtrato(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root_filtrato = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

void fixInsert_filtrato(NodePtr k){
		NodePtr u;
		while (k->parent->colore_filt == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->colore_filt == 1) {
					// case 3.1
					u->colore_filt = 0;
					k->parent->colore_filt = 0;
					k->parent->parent->colore_filt = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate_filtrato(k);
					}
					// case 3.2.1
					k->parent->colore_filt = 0;
					k->parent->parent->colore_filt = 1;
					leftRotate_filtrato(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->colore_filt == 1) {
					// mirror case 3.1
					u->colore_filt = 0;
					k->parent->colore_filt = 0;
					k->parent->parent->colore_filt = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate_filtrato(k);
					}
					// mirror case 3.2.1
					k->parent->colore_filt = 0;
					k->parent->parent->colore_filt = 1;
					rightRotate_filtrato(k->parent->parent);
				}
			}
			if (k == root_filtrato) {
				break;
			}
		}
		root_filtrato->colore_filt = 0;
	}


void insert_filtrato(NodePtr node) {

        NodePtr y = NULL;
		NodePtr x = root_filtrato;

		node->parent = NULL;
        node->left = TNULL;
		node->right = TNULL;
		node->colore_filt = 1; // new node must be red

		while (x != TNULL) {
			y = x;
			if (strncmp(node->str, x->str,k) < 0) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == NULL) {
			root_filtrato = node;
		} else if (strncmp(node->str, y->str,k) < 0) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == NULL){
			node->colore_filt = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == NULL) {
			return;
        }
		// Fix the tree
		fixInsert_filtrato(node);
	}

void fixDelete_filtrato(NodePtr x) {
		NodePtr s;
		while (x != root_filtrato && x->colore_filt == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->colore_filt == 1) {
					// case 3.1
					s->colore_filt = 0;
					x->parent->colore_filt = 1;
					leftRotate_filtrato(x->parent);
					s = x->parent->right;
				}

				if (s->left->colore_filt == 0 && s->right->colore_filt == 0) {
					// case 3.2
					s->colore_filt = 1;
					x = x->parent;
				} else {
					if (s->right->colore_filt == 0) {
						// case 3.3
						s->left->colore_filt = 0;
						s->colore_filt = 1;
						rightRotate_filtrato(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->colore_filt = x->parent->colore_filt;
					x->parent->colore_filt = 0;
					s->right->colore_filt = 0;
					leftRotate_filtrato(x->parent);
					x = root_filtrato;
				}
			} else {
				s = x->parent->left;
				if (s->colore_filt == 1) {
					// case 3.1
					s->colore_filt = 0;
					x->parent->colore_filt = 1;
					rightRotate_filtrato(x->parent);
					s = x->parent->left;
				}

				if (s->right->colore_filt == 0 && s->right->colore_filt == 0) {
					// case 3.2
					s->colore_filt = 1;
					x = x->parent;
				} else {
					if (s->left->colore_filt == 0) {
						// case 3.3
						s->right->colore_filt = 0;
						s->colore_filt = 1;
						leftRotate_filtrato(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->colore_filt = x->parent->colore_filt;
					x->parent->colore_filt = 0;
					s->left->colore_filt = 0;
					rightRotate_filtrato(x->parent);
					x = root_filtrato;
				}
			} 
		}
		x->colore_filt = 0;
	}

void deleteNodeHelper_filtrato(NodePtr node, NodePtr s) {
		// find the node containing key
		NodePtr x, y; 
		
        if(s->left==TNULL || s->right == TNULL){
            y = s;
        }else { 
            y = successor_filtrato(s); 
        }
        if(y->left!=TNULL) x = y->left;
        else x = y->right;

        x->parent= y->parent;
        
        if(y->parent==TNULL) root_filtrato = x;
        else if(y==y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        if(y!=s) {
            strncpy(s->str,y->str,k);
        }

		if (y->colore_filt == 0){
			fixDelete_filtrato(x);
		}
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

void rightRotate(NodePtr x) {
		NodePtr y = x->sx;
		x->sx = y->dx;
		if (y->dx != TNULL) {
			y->dx->p = x;
		}
		y->p = x->p;
		if (x->p == NULL) {
			radice = y;
		} else if (x == x->p->dx) {
			x->p->dx = y;
		} else {
			x->p->sx = y;
		}
		y->dx = x;
		x->p = y;
	}

void leftRotate(NodePtr x) {
		NodePtr y = x->dx;
		x->dx = y->sx;
		if (y->sx != TNULL) {
			y->sx->p = x;
		}
		y->p = x->p;
		if (x->p == NULL) {
			radice = y;
		} else if (x == x->p->sx) {
			x->p->sx = y;
		} else {
			x->p->dx = y;
		}
		y->sx = x;
		x->p = y;
	}

void fixInsert(NodePtr k){
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
						rightRotate(k);
					}
					// case 3.2.1
					k->p->colore = 0;
					k->p->p->colore = 1;
					leftRotate(k->p->p);
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
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->p->colore = 0;
					k->p->p->colore = 1;
					rightRotate(k->p->p);
				}
			}
			if (k == radice) {
				break;
			}
		}
		radice->colore = 0;
	}


NodePtr insert(char *stringa) {

        NodePtr node = malloc(sizeof(elemento));
        char *st = malloc(sizeof(char)*k);
        strncpy(st,stringa,k);

        NodePtr y = NULL;
		NodePtr x = radice;

        node->str = st;
		node->p = NULL;
        node->sx = TNULL;
		node->dx = TNULL;
		node->colore = 1; // new node must be red

		while (x != TNULL) {
			y = x;
			if (strncmp(node->str, x->str,k) < 0) {
				x = x->sx;
			} else {
				x = x->dx;
			}
		}
        printf("Arrivato\n");
		// y is parent of x
		node->p = y;
		if (y == NULL) {
			radice = node;
		} else if (strncmp(node->str, y->str,k) < 0) {
			y->sx = node;
		} else {
			y->dx = node;
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
		fixInsert(node);
    return node;
}

void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != radice && x->colore == 0) {
			if (x == x->p->sx) {
				s = x->p->dx;
				if (s->colore == 1) {
					// case 3.1
					s->colore = 0;
					x->p->colore = 1;
					leftRotate(x->p);
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
						rightRotate(s);
						s = x->p->dx;
					} 

					// case 3.4
					s->colore = x->p->colore;
					x->p->colore = 0;
					s->dx->colore = 0;
					leftRotate(x->p);
					x = radice;
				}
			} else {
				s = x->p->sx;
				if (s->colore == 1) {
					// case 3.1
					s->colore = 0;
					x->p->colore = 1;
					rightRotate(x->p);
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
						leftRotate(s);
						s = x->p->sx;
					} 

					// case 3.4
					s->colore = x->p->colore;
					x->p->colore = 0;
					s->sx->colore = 0;
					rightRotate(x->p);
					x = radice;
				}
			} 
		}
		x->colore = 0;
	}

void deleteNodeHelper(NodePtr node, NodePtr s) {
		// find the node containing key
		NodePtr x, y; 
		
        if(s->sx==TNULL || s->dx == TNULL){
            y = s;
        }else { 
            y = successor(s); 
        }
        if(y->sx!=TNULL) x = y->sx;
        else x = y->dx;

        x->p= y->p;
        
        if(y->p==TNULL) radice = x;
        else if(y==y->p->sx) y->p->sx = x;
        else y->p->dx = x;
        if(y!=s) {
            strncpy(s->str,y->str,k);
        }

		if (y->colore == 0){
			fixDelete(x);
		}
	}

/*
//INSERIMENTO BST FILTRATO NUOVO
int inserimento_tree_filtrato(elemento * *lista, elemento* parola){
    elemento * y = NULL;
    elemento * x = *(lista);

    int i = 0;
    while (x != NULL){
        i ++;
        y = x;
        //printf("Stringa di prova: %s\n", (*lista)->str);
        //printf("Parola in x: %s, parola in stringa: %s\n", x->str, st);
        //if(posizione(parola->str,x->str) == -1){ x = x->prev_bst; //prev è a sx
        if(strncmp(parola->str,x->str,k) < 0){ x = x->prev_bst;
        }
        else {x = x->next_bst; //next è a dx
        }
    }
    parola->testa = y;
    parola->next_bst = NULL;
    parola->prev_bst = NULL;

    if(y==NULL)
        *(lista) = parola;
    //else if(posizione(parola->str, y->str) == -1) {
    else if(strncmp(parola->str, y->str,k) < 0) {
        //printf("Dentro a sx");
        y->prev_bst=parola;}
    else {y->next_bst=parola; //printf("Dentro a dx");
    }

    return i;
}
*/

/*
void rotazione_sx(elemento *x, elemento_nil* lista){
    elemento * y = x->dx;
    //printf("Entra il nodo %s in sx_rotate\n", x->str);
    x->dx = y->sx;
    if(y->sx!=lista->nill) y->sx->p = x;
    y->p = x->p;
    if (x->p==lista->nill) lista->radice = y;
    else if(x==x->p->sx) x->p->sx = y;
    else x->p->dx = y;
    y->sx = x;
    x->p = y;
}
*/
/*
void rotazione_dx(elemento *y, elemento_nil* lista){
    elemento * x = y->sx;
    //printf("Entra il nodo %s in dx_rotate\n", x->str);
    y->sx = x->dx;
    if(x->dx!=lista->nill) x->dx->p = y;
    x->p = y->p;
    if (y->p==lista->nill) lista->radice = x;
    else if(y==y->p->dx) y->p->dx = x;
    else y->p->sx = x;
    x->dx = y;
    y->p = x;
}
*/
/*
void rb_inserimento_fixup(elemento_nil* lista, elemento *z){
    elemento * x= lista->nill;
    elemento * y= lista->nill;
    //printf("Colore z %d\n", z->colore);
    if(z==lista->radice) {
        lista->radice->colore=0; //balck
        //printf("Vuoto\n");
    }
    else{
        x = z->p;
        //printf("%s %d\n", x->str, x->colore);
        if(x->colore==1) { //rosso
            //printf("Dentro a rosso %s\n", x->str);
            if(x==x->p->sx){
                //printf("Primo colore %s\n", x->p->dx->str);
                y=x->p->dx;
                //printf("colore %d\n", y->colore);
                if(y->colore==1){
                    x->colore=0;
                    y->colore=0;
                    x->p->colore = 1;
                    //printf("Entra il nodo %s\n", z->str);
                    rb_inserimento_fixup(lista, x->p);
                }else{
                    //printf("In procinto di rotazione\n");
                    //printf("Stringa %s\n", x->dx->str);
                    if(z == x->dx){
                        z = x;
                        //printf("Rotazione a sx\n");
                        rotazione_sx(z,lista);
                        x=z->p;
                    }
                    x->colore=0;
                    //printf("Padre %s\n", x->p->str);
                    x->p->colore = 1;
                    //printf("Rotazione a dx\n");
                    rotazione_dx(x->p,lista);
                }
            }else{
                //printf("Due\n");
                y=x->p->dx; //vedere se mettere sx non mi da differenze  BELLA DOMANDA
                if(y->colore==1){
                    x->colore=0;
                    y->colore=0;
                    x->p->colore = 1;
                    rb_inserimento_fixup(lista, x->p);
                }else {
                    if(z == x->dx){
                        z = x;
                        rotazione_dx(z,lista);
                        x=z->p;
                    }
                    x->colore=0;
                    x->p->colore = 1;
                    rotazione_sx(x->p,lista);
                }
                
            }

        }
        //z->sx->colore = 0;  
    }
}
*/
/*
elemento * inserimento_tree(elemento_nil * lista, char* parola){
    //modifico per RB
    elemento * y = lista->nill;
    elemento * x = lista->radice;
    elemento * ell = malloc(sizeof(elemento));
    char *st = malloc(sizeof(char)*k);
    //scrittura(parola, st);
    strncpy(st,parola,k);
    
    printf("------------\n");
    printf("La radice: %s\n", lista->radice->str);
    printf("provas\n");
    
    //int i = 0;
    while (x!=lista->nill){
       //printf("Dentro\n");
        y = x;
        //printf("Stringa di prova: %s\n", (*lista)->str);
        //printf("Parola in x: %s, parola in stringa: %s\n", x->str, st);
        //printf("Pos: %d\n",posizione(st,x->str));
        //if(posizione(st,x->str) == -1){ 
        if(strncmp(st,x->str,k) < 0){
            x = x->sx; //printf("-1\n");
            //printf("-1_\n");
        }
        else {x = x->dx;
           // printf("1_\n");
        }
    }
    ell->p = y;
    ell->str = st;
    ell->sx = lista->nill;
    ell->dx = lista->nill;

    if(y==lista->nill)
        lista->radice = ell;
    //else if(posizione(ell->str, y->str) == -1) {
    else if(strncmp(ell->str, y->str,k) < 0) {  
        //printf("Dentro a sx");
        //printf("Dentro a sx");
        y->sx=ell;}
    else {y->dx=ell; 
        //printf("Dentro a dx");
    }

    //PARTE RB
    ell->colore=1; //colore rosso
    //printf("Inserito %s prima del fixup padre %s\n", parola, ell->p->str);
    rb_inserimento_fixup(lista,ell);
    //return ell;
    return ell;
}
*/
/*
elemento * three_minore(elemento *x){
    while(x->prev_bst!=NULL) x = x->prev_bst;
    return x;
}

elemento * successore_bst(elemento *s){
    elemento * y = NULL;
    if(s->next_bst!=NULL) return three_minore(s->next_bst);
    y = s->testa;
    while(y!=NULL && s==y->next_bst){
        s = y;
        y = y->testa;
    }
    return y;
}
*/
void scrittura_ordinata(elemento *x){
    if(x->dx!=NULL){    
        scrittura_ordinata(x->sx);
        //if(validazione(x->str,ver)) printf("%s\n", x->str);
        printf("%s\n", x->str);
        scrittura_ordinata(x->dx);
    }
}

//T->x e Z->s
void conto_ordinata(elemento *x,char *ver, int i){
    if(x!=NULL){    
        conto_ordinata(x->left,ver,i);
        if(validazione(x->str,ver)){
            if(i == 0) cont_buone ++;
            else printf("%s\n", x->str);
        }else{
            //elimino nell'albero
            deleteNodeHelper_filtrato(root_filtrato,x);
            //printf("Eliminazione: %s\n", x->str);

        }
        conto_ordinata(x->right,ver,i);
    }
}

void conto_ordinata_filtrato(elemento *x,char *ver, int i){
    if(x!=TNULL){    
        conto_ordinata_filtrato(x->sx,ver,i);
        if(validazione(x->str,ver)){
            if(i == 0) cont_buone ++;
            else printf("%s\n", x->str);
                //inserimento nel nuovo bst
            insert_filtrato(x); //occhio che me li inserisce già ordinati -> facendo così riduco solo il teta di n come numero
            //dovrei trasformare anche questo in RB
            
        }
        conto_ordinata_filtrato(x->dx,ver,i);
    }
}

/*
int uguale(char *c, char *p){
    for(int i = 0;i<k;i++)
        if(c[i] != p[i]) return 0;
    return 1;
}
*/

int controllo(NodePtr x, char * parola){
    printf("Dentro a controllo\n");
    //printf("%s\n", x->str);
    x = x->sx;
    printf("Prova\n");
    if(x!=TNULL){   
        //(x->str,parola)) return 1;
        if(!strncmp(x->str,parola,k)) return 1;
        else return (controllo(x->sx, parola) || controllo(x->dx,parola));
    }
    else {
        printf("Return 0 %s\n", x->str);
        return 0;
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
/*
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
}*/

int main(void){
    //elemento * lista = NULL;

    if(scanf("%d", &k) != EOF);

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
        if(scanf("%s", stringa)!=EOF);
        printf("Letto %s\n",stringa);
        //printf("Inserimento: %d\n", inserimento_tree(&lista,stringa));
        if(stringa[0] == '+'){
            if(stringa[1] == 'n'){
                radice = NULL; //TENERE D'OCCHIO

                nuova = 1;
                inserimento = 0;
                primo_inserimento = 1;
                //printf("INSERIMENTO PAROLE FINE\n");
                printf("INIZIO_PARTITA in +\n");
                pulisci(ver); //riazzero il dizionario
                //inserisco i nuovi elementi 
                if(scanf("%s", stringa)!=EOF);
                //scrittura(stringa,rif);
                strncpy(rif,stringa,k);
                if(scanf("%s", stringa)!=EOF);
                conteggio = atoi(&stringa[0]);
                printf("CONTEGGIO %d\n", conteggio);
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
                    conto_ordinata_filtrato(root_filtrato,ver,1);primo_inserimento = 0;}
                else {
                    //printf("NOn prima volta\n");
                    conto_ordinata(radice,ver,1); 
                }//faccio il conteggio sul nuovo bst
                //conto_ordinata(lista_filtrata,ver,1);
                //printf("S\n");
            }
        }else if(nuova){
            //insrriemnto durante la partita
            if(inserimento) {
                NodePtr prova = insert(stringa);
                //printf("Inserito nuove stringhe\n");
                if(validazione(stringa,ver)){
                    //se è gia valido lo metto li dentro
                    insert_filtrato(prova);
                }
            
            
            }else{
                //Qui dovrebbe partire algoritmo 
                //printf("Stringhe da confrontare %s\n", stringa);
                //VEDERE SE APPARTIENE O NO ALLE PAROLE AMMISSBILI
                //printf("CONTEGGIO prima -1: %D\n", conteggio);
                //printf("Stringa letta in : %s\n", stringa);
                //if(uguale(rif,stringa)){
                if(!strncmp(rif,stringa,k)){
                        printf("ok\n");
                        nuova = 0; //FINSICE LA PARTITA
                        conteggio = 0; //DA VERIFICARE
                        //rif[0] = '&';
                        //lista_filtrata = NULL; //pulisco il bst 
                        primo_inserimento = 1;
                }else if(!controllo(radice,stringa)){
                        printf("not_exists\n");
                }else if(conteggio != 1){ 
                    printf("Stringa letta in : %s\n", stringa);
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
                            conto_ordinata_filtrato(root_filtrato,ver,0);primo_inserimento = 0;}
                            //conto_ordinata_filtrato(lista.radice,&lista_filtrata,ver,0);primo_inserimento = 0;}
                        else {
                            //printf("NOn prima volta\n");
                            conto_ordinata(radice,ver,0); 
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
            printf("Inerito %s prima dell'ins\n", stringa);
            insert(stringa);
            printf("Inserito nuove stringhe\n");
        }
    }

    //scrittura_ordinata(lista);
    
    return 0;
}