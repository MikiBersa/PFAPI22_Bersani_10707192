#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k = 5;

typedef struct el{
    char *str;
    int colore; //0 è nero 1 è rosso
    struct el *sx;
    struct el *dx;
    struct el *p;
} elemento;

typedef struct {
    elemento *radice;
    elemento *nill;
}elemento_nil;

elemento nil = {"NULL", 0,NULL,NULL,NULL};
elemento nil2 = {"RADI", 0,NULL,NULL,NULL};

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

void rotazione_dx(elemento *y, elemento_nil* lista){
    elemento * x = y->sx;
    if(x != lista->nill){ //da eliminare
        printf("Entra il nodo %s in dx_rotate\n", x->str);
        y->sx = x->dx;
        if(x->dx!=lista->nill) x->dx->p = y;
        printf("Qui ci arrvio\n");
        x->p = y->p;
        if (y->p==lista->nill) lista->radice = x;
        else if(y==y->p->dx) y->p->dx = x;
        else y->p->sx = x;
        x->dx = y;
        y->p = x;
    }
}

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
        //z->sx->colore = 0;  
    }
}
int uguale(char *c, char *p){
    for(int i = 0;i<k;i++)
        if(c[i] != p[i]) return 0;
    return 1;
}

void inserimento_tree(elemento_nil *lista, char* parola){
    //modifico per RB
    elemento * y = lista->nill;
    elemento * x = lista->radice;
    elemento * ell = malloc(sizeof(elemento));
    char *st = malloc(sizeof(char)*k);
    scrittura(parola, st);
    /*
    printf("------------\n");
    printf("La radice: %s\n", lista->radice->str);
    printf("provas\n");
    */
    //int i = 0;
    while (x!=lista->nill){
       //printf("Dentro\n");
        y = x;
        //printf("Stringa di prova: %s\n", (*lista)->str);
        //printf("Parola in x: %s, parola in stringa: %s\n", x->str, st);
        //printf("Pos: %d\n",posizione(st,x->str));
        if(posizione(st,x->str) == -1){ 
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
    else if(posizione(ell->str, y->str) == -1) {
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
}

int contt = 0;
void scrittura_ordinata(elemento *x){
    if(x->dx!=NULL){    
        scrittura_ordinata(x->sx);
        //if(validazione(x->str,ver)) printf("%s\n", x->str);
        printf("%s\n", x->str); contt ++;
        scrittura_ordinata(x->dx);
    }
}

elemento * three_minore(elemento_nil* lista, elemento *x){
    //while(x->sx!=lista->nill) x = x->sx;
    while(x->sx!=lista->nill) {
        x = x->sx;
    }
    return x;
}

elemento * successore_bst(elemento_nil* lista, elemento *s){
    elemento * y = lista->nill;
    printf("Dentro a succ\n");
    if(s->dx!=lista->nill) {
        printf("Prima d minore\n");
        return three_minore(lista,s->dx);
    } 
    printf("NO s!=lista->nill\n");
    y = s->p;
    while(y!=lista->nill && s==y->dx){
        s = y;
        y = y->p;
    }
    return y;
}

void rb_delete_fixup(elemento_nil* lista, elemento *x){
    elemento * w = lista->nill;
    printf("colore x\n");
    //if(x->colore==1 || x->p==lista->nill){
    if(x->colore==1 || x->p==lista->nill || x == lista->nill){
        x->colore = 0;
    }else if(x==x->p->sx){
        printf("else if\n");
        w = x->p->dx;
        if(w->colore==1){
            w->colore = 0;
            x->p->colore = 1;
            rotazione_sx(x->p,lista);
            w = x->p->dx;
        }
        printf("w->sx->colore\n");
        printf("XXXX: %s\n", x->str);
        printf("w: %s\n", w->str);
        if(w==lista->nill) return;
        if(w->sx->colore == 0 && w->dx->colore==0){
            w->colore = 1;
            rb_delete_fixup(lista,x->p);
        }
        else {
            printf("else\n");
            if(w->dx->colore==0){
                w->sx->colore = 0;
                w->colore=1;
                rotazione_dx(w,lista);
                w = x->p->dx;
            }
            w->colore = x->p->colore;
            x->p->colore=0;
            w->dx->colore=0;
            rotazione_sx(x->p,lista);
        }
    }else{
        printf("Else\n");
        printf("X %s\n", x->str);
        w = x->p->dx;
        if(w->colore==1){
            printf("Rotazioene imminente\n");
            w->colore = 0;
            x->p->colore = 1;
            rotazione_dx(x->p,lista);
            w = x->p->sx;
        }
        printf("Avanti\n");
        printf("%s\n", w->str);
        printf("%s\n", w->dx->str);
        if(w->dx->colore == 0 && w->sx->colore==0){
            printf("Dentro\n");
            w->colore = 1;
            rb_delete_fixup(lista,x->p);
        }
        else {
            printf("Ancora\n");
            if(w->sx->colore==0){
                printf("Pre rotazione sx\n");
                w->dx->colore = 0;
                w->colore=1;
                rotazione_sx(w,lista);
                w = x->p->sx;
            }
            printf("Pre rotazione dx\n");
            printf("W %s\n",w->str);
            printf("x.p %s\n",x->p->str);
            printf("W.sx %s\n",w->sx->str);
            w->colore = x->p->colore;
            x->p->colore=0;
            w->sx->colore=0;
            rotazione_dx(x->p,lista);
        }

    }
}

//ELIMINAZIONE RB
elemento * eliminazione (elemento_nil* lista, elemento *s){
    elemento * y = lista->nill;
    elemento * x = lista->nill;

    printf("Dentro ad eliminazione\n");
    printf("prova %s\n", s->str);
    if(s->sx==lista->nill || s->dx == lista->nill){
        printf("y = s\n");
        y = s;
    }
    else {
        printf("Prima di successore\n");
        y = successore_bst(lista,s); 
    }//da fare questo algoritmo
    
    if(y->sx!=lista->nill) x = y->sx;
    else x = y->dx;

    printf("x->p = y->p\n");
    x->p = y->p;
    //if(x!=lista->nill) x->p = y->p;
    if(y->p==lista->nill) lista->radice = x;
    else if(y==y->p->sx) y->p->sx = x;
    else y->p->dx = x;
    printf("y!=s\n");
    if(y!=s) {
        printf("strncopy\n");
        //scrittura(y->str,s->str);
        strncpy(s->str,y->str,k);
    }
    printf("colore\n");
    if(y->colore==0){
        printf("Prima del fixup\n");
        printf("y %s\n", y->str);
        printf("x %s\n", x->str);
        rb_delete_fixup(lista,x);
    }

    //scrittura_ordinata(*(lista));
    return y;
}

int conto = 0;

void conto_ordinata_filtrato(elemento *x,elemento_nil *lista_nuova){
    //if(x!=lista_nuova->nill){   
    if(x->dx!=NULL){ 
        conto_ordinata_filtrato(x->sx,lista_nuova);
        
        if(conto < 1){ 
            conto ++; 
            printf("%s %d\n", x->str, conto);
        }
        else {
            conto = 0;
            printf("Eliminazione\n");
            //eliminazione(lista_nuova,x);
            printf("Eliminazione: %s\n", eliminazione(lista_nuova,x)->str);
        }
        
        conto_ordinata_filtrato(x->dx,lista_nuova);
    }
}



int main(void){
    char stringa[k];
    //elemento_nil lista = {NULL, &nil};
    elemento_nil lista = {&nil, &nil};
    //nil.colore = 0;
    while(!feof(stdin)){
        scanf("%s", stringa);
        printf("Stringa %s\n",stringa);
        inserimento_tree(&lista, stringa);
    }
    //rotazioni funzionano
    contt = 0;
    scrittura_ordinata(lista.radice);
    //printf("*****************\n");
    //printf("Conteggio %d\n", contt);
    //printf("_______________\n");
    //conto_ordinata_filtrato(lista.radice,&lista);
    //printf("________________\n");
    //contt = 0;
    //scrittura_ordinata(lista.radice);
    //printf("Conteggio %d\n", contt);
    return 0;
}