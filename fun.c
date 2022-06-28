#include <stdio.h>
#include <stdlib.h>

int main(void){
    /*
    char stringa[10];

    while(!feof(stdin)){
        scanf("%s", stringa);
        printf("%s", stringa);
    }
    */
    int x,y,r;

    r = scanf("%d %d", &x,&y);
    if(r==0)
        printf("Error");
    else
        printf("%d\n", x+y);

    return 0;
}