#include <stdio.h>
#include <stdlib.h>

int k = 5;

typedef struct el{
    char *str;
    int colore; //0 è nero 1 è rosso
    struct el *sx;
    struct el *dx;
    struct el *p;
} elemento;

void scrittura(char* c1, char *c2){ //da c1 a c2
    for(int i = 0; i<k;i++){
        c2[i] = c1[i];
    }
}

int posizione(char* c1, char *c2){ //c2 la testa
    for(int i = 0; i<k;i++){
        if(c1[i] > c2[i]) return 1; //destra
        else if(c1[i] < c2[i]) return -1; //sinistra
    }
    return 0;
}


void rotazione_sx(elemento *x, elemento **lista){
    elemento * y = x->dx;
    printf("Entra il nodo %s in sx_rotate\n", x->str);
    x->dx = y->sx;
    if(y->sx!=NULL) y->sx->p = x;
    y->p = x->p;
    if (x->p==NULL) *(lista) = y;
    else if(x==x->p->sx) x->p->sx = y;
    else x->p->dx = y;
    y->sx = x;
    x->p = y;
}

void rotazione_dx(elemento *y, elemento **lista){
    elemento * x = y->sx;
    printf("Entra il nodo %s in dx_rotate\n", x->str);
    y->sx = x->dx;
    if(x->dx!=NULL) x->dx->p = y;
    x->p = y->p;
    if (y->p==NULL) *(lista) = x;
    else if(y==y->p->dx) y->p->dx = x;
    else y->p->sx = x;
    x->dx = y;
    y->p = x;
}

void rb_inserimento_fixup(elemento * *lista, elemento *z){
    elemento * x= NULL;
    elemento * y= NULL;
    if(z==*(lista)) z->colore=0; //balck
    else{
        x = z->p;
        if(x->colore==1) { //rosso
            if(x==x->p->sx){
                y=x->p->dx;
                if(y->colore==1){
                    x->colore=0;
                    y->colore=0;
                    x->p->colore = 1;
                    printf("Entra il nodo %s\n", z->str);
                    rb_inserimento_fixup(lista, x->p);
                }else {
                    if(z == x->dx){
                        z = x;
                        rotazione_sx(z,lista);
                        x=z->p;
                    }
                    x->colore=0;
                    x->p->colore = 1;
                    rotazione_dx(x->p,lista);
                }
            }else{
                y=x->p->dx;
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
    }
}

elemento * inserimento_tree(elemento * *lista, char* parola){
    //modifico per RB
    elemento * y = NULL;
    elemento * x = *(lista);
    elemento * ell = malloc(sizeof(elemento));
    char *st = malloc(sizeof(char)*k);
    scrittura(parola, st);

    //int i = 0;
    while (x != NULL){
        //i ++;
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
    else {y->dx=ell; }

    //PARTE RB
    //ell->colore=1; //colore rosso
    printf("Inerito %s prima del fixup\n", parola);
    //rb_inserimento_fixup(lista,ell);
    return ell;
}

void scrittura_ordinata(elemento *x){
    if(x!=NULL){    
        scrittura_ordinata(x->dx);
        //if(validazione(x->str,ver)) printf("%s\n", x->str);
        printf("%s\n", x->str);
        scrittura_ordinata(x->sx);
    }
}


int main(void){
    char * stringa = NULL;
    elemento * lista = NULL;
    while(!feof(stdin)){
        scanf("%s", stringa);
        printf("Stringa %s ",stringa);
        inserimento_tree(&lista, stringa);
    }

    //scrittura_ordinata(lista);

    //rotazione_dx(lista->dx,&lista);

    //scrittura_ordinata(lista);

    return 0;
}