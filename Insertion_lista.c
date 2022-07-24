#include <stdio.h>
#include <stdlib.h>

typedef struct el{
    int numero;
    struct el* next;
    struct el* prev;
}elemento;

typedef elemento * NodePtr;
typedef struct {
    NodePtr radice_lista;
    NodePtr fine_lista;
}Root;

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


void inserimento(int num , Root *root){
    NodePtr elem;
    elem = malloc(sizeof(elemento));
    elem->numero = num;

    inserisci_lista(root,elem);
}

void stampa(NodePtr l){
    while(l!=NULL) {
        printf("%d ", l->numero);
        l = l->next;
    }
}




int main(){
    Root root = {NULL,NULL};
    inserimento(5,&root);
    inserimento(7,&root);
    inserimento(8,&root);
    inserimento(2,&root);
    inserimento(3,&root);
    inserimento(1,&root);

    printf("STAMPO LA LISTA\n");
    stampa(root.radice_lista);
    return 0;
}