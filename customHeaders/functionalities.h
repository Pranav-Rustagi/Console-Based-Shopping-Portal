/******************************************\
|    delays program for ms milliseconds    |
\******************************************/
void delay(int ms) {
    clock_t start_time = clock();
    while (clock() < start_time + ms);
}




/********************\
|   clears console   |
\********************/
void cls() {
    printf("\e[1;1H\e[2J");
}



/***********************************************************\
|   prints msg string, and does dot animation for loading   |
\***********************************************************/
void loadingAnimation(char *msg, bool clearScr) {
    if(clearScr)
        cls();
    printf("\n\n%s", msg);
    for(int i = 0 ; i < 6 ; i++) {
        if(i == 3)
            printf("\b\b\b   \b\b\b");

        delay(200);
        printf(".");
    }
    if(clearScr)
        cls();
}




/*****************************\
|   pressing key to go back   |
\*****************************/
void backToDashboard() {
    printf("\n\nPress ENTER to go back to dashboard.");
    getch();
    fflush(stdin);
}



/*****************************\
|   checks if file is empty   |
\*****************************/
bool isFileEmpty(FILE *file) {
    fseek(file, 0, SEEK_END);
    return (ftell(file) == 0);
}