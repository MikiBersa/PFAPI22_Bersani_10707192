void insert(char *stringa, NodePtr *radicec, int validazione, Root *root, int conf) {
        NodePtr node = malloc(sizeof(elemento)); //creo il posto per il nodo in memoria
        //char *st = malloc(sizeof(char)*k); //creo il posto per la parola nel nodo

        NodePtr y = NULL;
		NodePtr x = *(radicec);
        NodePtr register ult_valido = NULL;
        short int posi = 2;
        //scrittura(stringa,st); //trasferisco la parola di puntatore
        //node->str = st;
        for(int i = 0; i<k;i++) node->str[i] = stringa[i];
        node->sx = TNULL;
		node->dx = TNULL;
		node->colore = 1; // nuovo nodo di rosso
        node->valida = validazione; //così lo tengo traccia per dopo
        //0 di default se parte da zero il gioco 
        
        node->p = NULL;

		while (x != TNULL) {

            if(x->valida) {
                ult_valido = x; //vado a vedere ultimo valido così da inserire la parola in maniera corretta nella lista
                //MA QUI VEDO SOLO QUELLI CHE SONO NELLA MIA LINEA DI ALBERO QUELLI ESTERNI VANNO CERCARTI 
                //VEDI IL CASO CON ULT_VALIDO == NULL
            }

			y = x;

            if (posizione(node->str, x->str) < 0) {
				x = x->sx;
                if(y == ult_valido) posi = -1;
			} else {
				x = x->dx;
                if(y == ult_valido) posi = 1;
			}
		}

		node->p = y;

		if (y == NULL) {
			*(radicec) = node;
        } else if (posizione(node->str, y->str) < 0) {
            #ifdef debug2
            if(ult_valido!=NULL) printf("confronto padre %s sx %s ultimo %s posi %d\n",y->str, node->str, ult_valido->str, posi);
			else printf("confronto padre %s sx %s ultimo NULL posi %d\n",y->str, node->str, posi);
            #endif
            y->sx = node;
            //QUI VUOL DIRE CHE STA A SX
            if(validazione && conf){
                #ifdef debug2
                printf("SX %s\n", node->str);
                #endif
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
                    //inserisco in testa
                    #ifdef debug2
                    printf("ULTIMO NULL\n");
                    printf("%s puntato da root\n", root->radice_lista->str);
                    printf("%s puntato da root fine\n", root->fine_lista->str);
                    #endif
                    //HO INVERTITO I PUNTATORI
                    node->next = NULL;
                    node->prev = root->fine_lista;

                    if(root->fine_lista!=NULL) root->fine_lista->next = node;
                    else root->radice_lista = node;
                    root->fine_lista = node;
                    
                    /*
                    node->next = root->radice_lista;
                    node->prev = NULL;
                    if(root->radice_lista!=NULL) root->radice_lista->prev = node;
                    else root->fine_lista = node;
                    root->radice_lista = node;
                    */
                }

            }

		} else {
            #ifdef debug2
            if(ult_valido!=NULL) printf("confronto padre %s dx %s ultimo %s posi %d\n",y->str, node->str, ult_valido->str, posi);
			else printf("confronto padre %s dx %s ultimo NULL posi %d\n",y->str, node->str, posi);
            #endif
			y->dx = node;
            //QUI VUOL DIRE CHE STA A DX
            if(validazione && conf){
                #ifdef debug2
                printf("DX %s posi %d\n", node->str, posi);
                #endif
                if(posi == 1){
                    if(ult_valido != NULL){
                        #ifdef debug2
                        printf("ultimo %s\n", ult_valido->str);
                        printf("%s puntato da root\n", root->radice_lista->str);
                        printf("%s puntato da root fine\n", root->fine_lista->str);
                        #endif
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

                        if(root->radice_lista!=NULL) root->radice_lista->prev = node;
                        else root->fine_lista = node;

                        root->radice_lista = node;

                    }
                }else if(posi == -1){
                        #ifdef debug2
                        printf("Ultimo valido %s in posi %d\n", ult_valido->str, posi);
                        #endif
                        if(ult_valido != NULL){
                            node->next = ult_valido->next;

                            if(ult_valido->next != NULL) {
                                node->prev = ult_valido->next->prev;
                                ult_valido->next->prev = node;
                                ult_valido->next = node;
                            }
                            if(ult_valido->next == NULL) {
                                #ifdef debug2
                                printf("Next nullo\n");
                                #endif
                                node->prev = ult_valido;
                                ult_valido->next = node;
                                root->fine_lista = node;
                            }
                    }else{
                        //inserisco in testa
                        #ifdef debug2
                        printf("ULTIMO NULL\n");
                        printf("%s puntato da root\n", root->radice_lista->str);
                        printf("%s puntato da root fine\n", root->fine_lista->str);
                        #endif
                        //HO INVERTITO I PUNTATORI
                        node->next = NULL;
                        node->prev = root->fine_lista;

                        if(root->fine_lista!=NULL) root->fine_lista->next = node;
                        else root->radice_lista = node;
                        root->fine_lista = node;
                        
                        /*
                        node->next = root->radice_lista;
                        node->prev = NULL;
                        if(root->radice_lista!=NULL) root->radice_lista->prev = node;
                        else root->fine_lista = node;
                        root->radice_lista = node;
                        */
                    }
                    
                }
            } 
		}
		if (node->p == NULL){
			node->colore = 0;
			return;
		}

		if (node->p->p == NULL) {
			return;
        }
		fixInsert(node, radicec);
}