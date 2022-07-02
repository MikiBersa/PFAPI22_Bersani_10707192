#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int k = 5;

typedef struct Node {
	char* str; // stringa key
	struct Node *parent; // pointer to the parent
	struct Node *left; // pointer to left child
	struct Node *right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
}Node;

typedef Node *NodePtr;

NodePtr root;
NodePtr TNULL;

void init_rb(){
    TNULL = malloc(sizeof(Node));
    TNULL->color=0;
    TNULL->left=NULL;
    TNULL->right=NULL;
    root = TNULL;
}


NodePtr maximum(NodePtr node) {
	while (node->right != TNULL) {
		node = node->right;
	}
	return node;
}

NodePtr minimum(NodePtr node) {
	while (node->left != TNULL) {
		node = node->left;
	}
	return node;
}

NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
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

void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

void fixInsert(NodePtr k){
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

void insert(char *stringa) {
		// Ordinary Binary Search Insertion
        NodePtr node = malloc(sizeof(Node));
        char *st = malloc(sizeof(char)*k);
        strncpy(st,stringa,k);

		node->parent = NULL;

		node->str = st;
		
        node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		NodePtr y = NULL;
		NodePtr x = root;

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
			root = node;
		} else if (strncmp(node->str, y->str,k) < 0) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == NULL){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == NULL) {
			return;
        }
		// Fix the tree
		fixInsert(node);
	}

int conteggio = 0;
void inOrder(NodePtr node) {
		if (node != TNULL) {
			inOrder(node->left);
			printf("%s\n", node->str); conteggio ++;
			inOrder(node->right);
		} 
}

void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = 0;
	}

void deleteNodeHelper(NodePtr node, NodePtr s) {
		// find the node containing key
		NodePtr x, y; 
		
        if(s->left==TNULL || s->right == TNULL){
            y = s;
        }else { 
            y = successor(s); 
        }
        if(y->left!=TNULL) x = y->left;
        else x = y->right;

        x->parent= y->parent;
        
        if(y->parent==TNULL) root = x;
        else if(y==y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        if(y!=s) {
            strncpy(s->str,y->str,k);
        }

		if (y->color == 0){
			fixDelete(x);
		}
	}

int cont = 0;
void mood(NodePtr node) {
		if (node != TNULL) {
			mood(node->left);
            if(cont < 1) {
                printf("%s\n", node->str);
                cont ++;
            }else{
                cont = 0;
                printf("Eliminata: %s\n", node->str);
                deleteNodeHelper(root,node);
            }
			mood(node->right);
		} 
}

int main(void){
    //RBTree bst;
    char stringa[k];
    init_rb();
    //nil.colore = 0;
    while(!feof(stdin)){
        scanf("%s", stringa);
        //printf("Stringa %s\n",stringa);
        insert(stringa);
    }

    printf("IN ordine\n");
    inOrder(root);
    printf("Conteggio : %d\n", conteggio);
    printf("******************** ELIMINAZIONE ******************\n");
    mood(root);
    printf("IN ORDINE\n");
    conteggio = 0;
    inOrder(root);
    printf("Conteggio : %d\n", conteggio);


    return 0;   
}