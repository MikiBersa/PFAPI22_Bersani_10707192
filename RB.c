#include <stdio.h>
#include <stdlib.h>

int k = 5;
/*
typedef enum {rosso, nero} colore_t;

typedef struct nodo
{
  char* str;
  colore_t colore;
  struct nodo *sinistro, *destro, *padre;
} alberorn_t;  

void rotazione_sx(alberorn_t *x, alberorn_t *lista){
    alberorn_t * y = x->destro;
    //printf("Entra il nodo %s in sx_rotate\n", x->str);
    x->destro = y->sinistro;
    if(y->sinistro!=NULL) y->sinistro->padre = x;
    y->padre = x->padre;
    if (x->padre==NULL) lista = y;
    else if(x==x->padre->sinistro) x->padre->sinistro = y;
    else x->padre->destro = y;
    y->sinistro = x;
    x->padre = y;
}

void rotazione_dx(alberorn_t *y, alberorn_t *lista){
    alberorn_t * x = y->sinistro;
    //printf("Entra il nodo %s in dx_rotate\n", x->str);
    y->sinistro = x->destro;
    if(x->destro !=NULL) x->destro->padre = y;
    x->padre = y->padre;
    if (y->padre==NULL) lista = x;
    else if(y==y->padre->destro) y->padre->destro = x;
    else y->padre->sinistro = x;
    x->destro = y;
    y->padre = x;
}

void ripristina(alberorn_t* sent, alberorn_t* nodo)
{
  alberorn_t *zio;
  
  while(nodo->padre->colore == rosso)
  {
     if(nodo->padre == nodo->padre->padre->sinistro)
     {
        zio = nodo->padre->padre->destro;
        if(zio->colore == rosso)
        {
           nodo->padre->colore = nero;
           zio->colore = nero;
           nodo->padre->padre->colore = rosso;
           nodo = nodo->padre->padre;           
        }
        else
        {
           if(nodo == nodo->padre->destro)
           {
              nodo = nodo->padre;
              rotazione_sx(nodo,sent);
           }
           nodo->padre->colore = nero;
           nodo->padre->padre->colore = rosso;
           rotazione_dx(nodo->padre->padre,sent);           
        }
     }
     else
     {
        zio = nodo->padre->padre->sinistro;
        if(zio->colore == rosso)
        {
           nodo->padre->colore = nero;
           zio->colore = nero;
           nodo->padre->padre->colore = rosso;
           nodo = nodo->padre->padre;           
        }
        else
        {
           if(nodo == nodo->padre->sinistro)
           {
              nodo = nodo->padre;
              rotazione_dx(nodo,sent);
           }
           nodo->padre->colore = nero;
           nodo->padre->padre->colore = rosso;
           rotazione_sx(nodo->padre->padre,sent);           
        }
     }
  }
  sent->sinistro->colore = nero;  
}

void scrittura(char* c1, char *c2){ //da c1 a c2
    for(int i = 0; i<k;i++){
        c2[i] = c1[i];
    }
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

int inserisci(alberorn_t* sent, char* parola)
{
  int inserito;
  alberorn_t *nodo, *padre, *nuovo;
  for(nodo = sent->sinistro, padre = sent; ((nodo != sent) && (uguale(nodo->str,parola)));

      padre = nodo, nodo = (posizione(nodo->str,parola))?
                             nodo->sinistro:
                             nodo->destro);

  if(nodo != sent)
  {
     inserito = 0;
  }
  else
  {
     inserito = 1;
     nuovo = (alberorn_t *)malloc(sizeof(alberorn_t));
     char *st = malloc(sizeof(char)*k);
    scrittura(parola, st);
     nuovo->str = st;
     nuovo->colore = rosso;
     nuovo->sinistro = nuovo->destro = sent;
     nuovo->padre = padre;

     if(padre == sent)
     {
        sent->sinistro = sent->destro = nuovo;
     }
     else
     {
        if(posizione(padre->str, parola))
        {
           padre->sinistro = nuovo;
        }
        else
        {
           padre->destro = nuovo;
        }
     }
     ripristina(sent, nuovo);
  }
  return(inserito);
}

void scrittura_ordinata(alberorn_t *x){
    if(x!=NULL){    
        scrittura_ordinata(x->sinistro);
        //if(validazione(x->str,ver)) printf("%s\n", x->str);
        printf("%s\n", x->str);
        scrittura_ordinata(x->destro);
    }
}
*/


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

void scrittura_ordinata(elemento *x){
    if(x!=NULL){    
        scrittura_ordinata(x->sx);
        //if(validazione(x->str,ver)) printf("%s\n", x->str);
        if(x->dx!=NULL) printf("%s\n", x->str);
        scrittura_ordinata(x->dx);
    }
}


int main(void){
    char stringa[k];
    //elemento_nil lista = {NULL, &nil};
    elemento_nil lista = {&nil, &nil};
    nil.colore = 0;
    while(!feof(stdin)){
        scanf("%s", stringa);
        //printf("Stringa %s\n",stringa);
        inserimento_tree(&lista, stringa);
    }
    //rotazioni funzionano
    scrittura_ordinata(lista.radice);
    return 0;
}