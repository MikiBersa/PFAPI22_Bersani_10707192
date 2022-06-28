//FUNZIONA VERSIONE CON I CICLI FOR SPERANDO CHE LE LUNGHEZZE DELLE PAROLE SIA CORTA

#include <stdio.h>

#define N 16

int main(void){
    char str1[N];
    char str2[N];
    char out[N];

    scanf("%s %s", str2,str1);

    for(int i = 0; i<N-1;i++){
        if(str1[i] == str2[i]) out[i] = '+';
        else{
            for(int j = 0; j<N-1;j++){
                if(str1[i] == str2[j] && i!=j) { out[i] = '|'; break;}
            }
            if(out[i] != '|') out[i] = '/';
        }
    }

    for(int i = 0; i<N-1; i++){
        if(out[i]=='|'){
            int cont = 0;
            int giu = 0;
            char c = str1[i];
            for(int j = 0; j<N-1;j++){
                if(str2[j]== str1[i]) cont ++;
                if(str1[j] == c && out[j] == '+') giu ++;
            }
            cont = cont - giu;
            giu = 0;
            for(int k = i-1; k>=0; k--){
                if(str1[k] == c && out[k] == '|') giu ++;
            }
            if(giu >= cont || cont == 0) out[i] = '/';
        }
    }
    printf("%s\n", out);

}