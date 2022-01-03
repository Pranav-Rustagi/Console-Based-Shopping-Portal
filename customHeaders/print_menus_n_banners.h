/***************************\
|   prints dashboard menu   |
\***************************/
void printMainMenu(bool loggedIn) {
    printBoxEdge();
    printBoxField("");
    printBoxField("MENU");
    printBoxField("");
    printBoxField("1. Display Items");
    if(loggedIn) {
        printBoxField("2. Cart");
        printBoxField("3. Sign Out");
    }
    else {
        printBoxField("2. Sign Up");
        printBoxField("3. Sign In");
    }
    printBoxField("4. Contact Us");
    printBoxField("5. Exit portal");
    printBoxField("");
    printBoxEdge();
}




/***************************\
|   prints sign up banner   |
\***************************/
void printSignUpBanner() {
    printBoxEdge();
    printBoxField("");
    printBoxField("Sign Up");
    printBoxField("");
    printBoxField("Please enter details");
    printBoxField("");
    printBoxEdge();
}




/***************************\
|   prints sign up banner   |
\***************************/
void printSignInBanner() {
    printBoxEdge();
    printBoxField("");
    printBoxField("Sign In");
    printBoxField("");
    printBoxField("Please enter details");
    printBoxField("");
    printBoxEdge();
}




/***************************\
|   prints sign up banner   |
\***************************/
void printProductPageBanner() {
    printBoxEdge();
    printBoxField("");
    printBoxField("Product Page");
    printBoxField("");
    printBoxEdge();
}




void printCartPageBanner() {
    printBoxEdge();
    printBoxField("");
    printBoxField("My Cart");
    printBoxField("");
    printBoxEdge();
}




void printCartPageMenu() {
    printBoxEdge();
    printBoxField("");
    printBoxField("MENU");
    printBoxField("");
    printBoxField("1. Place Order");
    printBoxField("2. Delete an item from cart");
    printBoxField("3. Go back to dashboard");
    printBoxField("");
    printBoxEdge();
}