#define BOX_WIDTH 50

/*************************\
|   prints box contents   |
\*************************/
void printBoxEdge() {
    printf("\n");
    for(int i = 0 ; i < BOX_WIDTH ; i++)
        printf("*");
}

void printBoxField(char *str) {
    printf("\n*   %s%*c", str, BOX_WIDTH - strlen(str) - 4, '*');
}


void printDashedLine(int width) {
    for(int i = 0 ; i < width ; i++)
        printf("-");
}