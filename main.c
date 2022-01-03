#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <conio.h>

// custom header files
#include "./customHeaders/functionalities.h"
#include "./customHeaders/printfields.h"
#include "./customHeaders/print_menus_n_banners.h"

#define arrBlockIncSize 50


/******************************\
|    Structure for User Type   |
\******************************/
typedef struct Item { 
    unsigned int id;
    char name[50];
    float mrp;
    float gst;
    float discount;
} Item;

typedef struct Order {
    char userID[50];
    unsigned int itemCount;
    unsigned int items[100];
} Order;

typedef struct User {
    char name[50];
    char address[100];
    char mobile[11];
    char username[50];
    char email[50];
    char password[25];
    int orderArrInd;
} User;




/***********************\
|    Global Variables   |
\***********************/
Item *itemArr = NULL;
Order *orderArr = NULL;
int itemArrLen = 0, orderArrLen = 0;



// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //





/***************************************\
|    compare function to sort itemArr   |
\***************************************/
int cmpforItems(const void * a, const void * b) {
    return (((Item *)a) -> id) - (((Item *)b) -> id);
}


/**************************\
|    Initializes itemArr   |
\**************************/
void populateItemArr(FILE *items) {
    fseek(items, 0, SEEK_SET);
    itemArr = (Item *) malloc(arrBlockIncSize * sizeof(Item));
    Item tmpItem;
    while(fgets(tmpItem.name, 50, items)) {
        tmpItem.name[strlen(tmpItem.name) - 1] = '\0';
        fscanf(items, "%u %f %f\n", &tmpItem.id, &tmpItem.mrp, &tmpItem.discount);
        if(tmpItem.mrp > 500)
            tmpItem.gst = 18;
        itemArr[itemArrLen++] = tmpItem;
        if((itemArrLen % arrBlockIncSize) == 0)
            itemArr = (Item *) realloc(itemArr, (itemArrLen + arrBlockIncSize) * sizeof(Item));
    }

    qsort(itemArr, itemArrLen, sizeof(Item), cmpforItems);
}





/**************************************\
|    compare function to sort orders   |
\**************************************/
int cmpforOrderItems(const void * a, const void * b) {
    return (*((int *)a) - *((int *)b));
}




/**************************\
|    Initializes itemArr   |
\**************************/
void populateOrderArr(FILE *orders) {
    fseek(orders, 0, SEEK_SET);
    orderArr = (Order *) malloc(arrBlockIncSize * sizeof(Order));
    Order tmpOrder;
    while(fread(&tmpOrder, sizeof(Order), 1, orders)) {
        qsort(tmpOrder.items, tmpOrder.itemCount, sizeof(unsigned int), cmpforOrderItems);
        orderArr[orderArrLen++] = tmpOrder;
        if((orderArrLen % arrBlockIncSize) == 0)
            orderArr = (Order *) realloc(orderArr, (orderArrLen + arrBlockIncSize) * sizeof(Order));
    }
}





/************************************************************\
|   returns user record having email as same as specified    |
\************************************************************/
User* getUser(char email[], FILE *users) {
    User u;
    fseek(users, 0, SEEK_SET);
    while(fread(&u, sizeof(User), 1, users))
        if(strcmpi(email, u.email) == 0) {
            User *res = malloc(sizeof(User));
            strcpy(res -> name, u.name);
            strcpy(res -> address, u.address);
            strcpy(res -> mobile, u.mobile);
            strcpy(res -> username, u.username);
            strcpy(res -> email, u.email);
            strcpy(res -> password, u.password);
            return res;
        }
    return NULL;
}





/*******************************************************************\
|   returns order record index having email as same as specified    |
\*******************************************************************/
int getOrders(char email[]) {
    for(int i = 0 ; i < orderArrLen ; i++)
        if(strcmpi(email, orderArr[i].userID) == 0)
            return i;

    Order newOrder;
    newOrder.itemCount = 0;
    strcpy(newOrder.userID, email);
    orderArr[orderArrLen] = newOrder;
    
    if(((orderArrLen + 1) % arrBlockIncSize) == 0)
        orderArr = (Order *) realloc(orderArr, (orderArrLen + 1 + arrBlockIncSize) * sizeof(Order));

    return orderArrLen++;
}





// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //




/*******************\
|   Saving Orders   |
\*******************/
void saveOrders() {
    loadingAnimation("Saving data.\nPlease wait.", true);

    FILE *orders = fopen(".\\data\\orders.txt", "w");
    for(int i = 0 ; i < orderArrLen ; i++)
        if(orderArr[i].itemCount > 0)
            fwrite(orderArr + i, sizeof(Order), 1, orders);
    fclose(orders);
}





/************************\
|   Deletes Order Item   |
\************************/
void deleteItemFromCart(unsigned int itemId, unsigned int orderArrInd) {
    int length = orderArr[orderArrInd].itemCount;
    int iter = 0;
    for(int i = 0 ; i < length ; i++) {
        unsigned int tmpId = orderArr[orderArrInd].items[i];
        if(tmpId != itemId)
            orderArr[orderArrInd].items[iter++] = tmpId;
    }

    orderArr[orderArrInd].itemCount = iter;
}




// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //





/**********************\
|   Displaying Items   |
\**********************/
void displayItems(User *user, FILE *orders) {
    loadingAnimation("Loading product page", true);
    printProductPageBanner();
    printf("\n\n\n");
    if(itemArrLen == 0)
        printf("No items found!");
    else {
        printDashedLine(105);
        printf("\n|  %7s   |  %-25s   |  %10s   |  %8s   |  %7s   |  %12s  |\n", "Item ID", "Item Name", "MRP (Rs.)", "Discount", "GST", "Total (Rs.)");
        printDashedLine(105);

        for(int i = 0 ; i < itemArrLen ; i++) {
            Item item = itemArr[i];
            float discounted = (item.mrp - item.mrp * (item.discount / 100));
            float total = discounted + discounted * (item.gst / 100);

            printf("\n|  %7d   |  %-25s   |  %10g   |  ", item.id, item.name, item.mrp);
            if(item.discount)
                printf("%7g%%", item.discount);
            else
                printf("%8s", "--");
            
            printf("   |  ");

            if(item.gst)
                printf("%6g%%", item.gst);
            else
                printf("%7s", "--");
            
            if(total == ceil(total))
                printf("   |  %12g  |", total);
            else
                printf("   |  %12.2f  |", total);
        }

        printf("\n");
        printDashedLine(105);
        printf("\n\n");

        if(user != NULL && orders != NULL) {
            bool backToDashboard = false, itemAdded = false;
            char choice;
            int id, tmpInd, tmpCount;

            do {
                printf("\n\nDo you want to add an item to the cart? (PRESS \'N\' to exit) : ");
                scanf("\n%c", &choice);
                if(choice == 'N' || choice == 'n') {
                    saveOrders();
                    if(itemAdded)
                        printf("Items added to cart successfully.");
                    backToDashboard = true;
                }
                else {
                    printf("\nEnter item ID : ");
                    scanf("%d", &id);
                    tmpInd = user -> orderArrInd;
                    tmpCount = orderArr[tmpInd].itemCount;
                    orderArr[tmpInd].items[tmpCount] = id;
                    (orderArr[tmpInd].itemCount)++;

                    if(!itemAdded)
                        itemAdded = true;
                }
            } while(!backToDashboard);
        }
    }
}







/*************************\
|   Displays Cart Items   |
\*************************/
void viewCart(User *loggedInUser) {
    loadingAnimation("Loading cart page", true);
    printCartPageBanner();
    printf("\n\n\n");
    printDashedLine(105);
    printf("\n|  %7s   |  %-25s   |  %10s   |  %8s   |  %7s   |  %12s  |\n", "Item ID", "Item Name", "MRP (Rs.)", "Discount", "GST", "Total (Rs.)");
    printDashedLine(105);

    Order userOrder = orderArr[loggedInUser->orderArrInd];
    qsort(userOrder.items, userOrder.itemCount, sizeof(unsigned int), cmpforOrderItems);

    double totalBill = 0;
    for(int i = 0 ; i < userOrder.itemCount ; i++) {
        Item *item = (Item *) bsearch(&(userOrder.items[i]), itemArr, itemArrLen, sizeof(Item), cmpforItems);
        float discounted = ((item -> mrp) - ((item -> mrp) * ((item -> discount) / 100)));
        float total = discounted + discounted * (item -> gst / 100);

        totalBill += total;

        printf("\n|  %7d   |  %-25s   |  %10g   |  ", item -> id, item -> name, item -> mrp);
        if(item -> discount)
            printf("%7g%%", item -> discount);
        else
            printf("%8s", "--");

        printf("   |  ");
        
        if(item -> gst)
            printf("%6g%%", item -> gst);
        else
            printf("%7s", "--");
        
        if(total == ceil(total))
            printf("   |  %12g  |", total);
        else
            printf("   |  %12.2f  |", total);
    }

    printf("\n");
    printDashedLine(105);
    printf("\n|  Total Bill %74c  %12g  |\n", '|', ceil(totalBill));
    printDashedLine(105);
    printf("\n\n");

    char choice = '0';
    bool backToDashboard = false, itemRemoved = false;

    do {
        if(choice == '0')
            printCartPageMenu();

        printf("\n\nEnter choice: ");
        scanf("\n%c", &choice);
        getc(stdin);

        switch(choice) {
            case '1': {
                char billLbl[20] = "Total Bill : ";
                char totalBillAmt[100];
                itoa(totalBill, totalBillAmt, 10);
                strcat(billLbl, totalBillAmt);

                loadingAnimation("Placing Order", true);
                orderArr[loggedInUser->orderArrInd].itemCount = 0;
                saveOrders();
                printf("Order placed successfully!\nThank you for shopping.");
                printf("\n\nPlease find the bill below\n");
                printBoxEdge();
                printBoxField("");
                printBoxField(billLbl);
                printBoxField("");
                printBoxEdge();
                break;
            }

            case '2': {
                unsigned int itemId;
                printf("\nEnter id of item to be deleted : ");
                scanf("%u", &itemId);
                deleteItemFromCart(itemId, loggedInUser -> orderArrInd);

                if(!itemRemoved)
                    itemRemoved = true;
                break;
            }

            case '3': {
                if(itemRemoved) {
                    loadingAnimation("Saving data", true);
                    saveOrders();
                    loadingAnimation("\n\nGoing back to the dashboard", false);
                }
                break;
            }

            default: printf("\nERROR : Invalid Choice! Enter a valid choice.");            
        }



        if(choice == '2')
            choice = '0';
        else if(choice == '1' || choice == '3')
            backToDashboard = true;

    } while(!backToDashboard);
}








/***************************************************\
|   takes input for signing up, and saves to data   |
\***************************************************/
void registerUser(FILE *users) {
    loadingAnimation("Loading sign up page", true);
    printSignUpBanner();

    User p;
    printf("\n\nEnter name : ");
    fgets(p.name, 50, stdin);
    p.name[strlen(p.name) - 1] = '\0';
    printf("Enter address : ");
    fgets(p.address, 100, stdin);
    p.address[strlen(p.address) - 1] = '\0';
    printf("Enter mobile number : ");
    scanf("\n%s", &(p.mobile));
    printf("Enter username : ");
    scanf("\n%s", &(p.username));
    printf("Enter email : ");
    scanf("\n%s", &(p.email));
    printf("Enter password : ");
    scanf("\n%s", &(p.password));


    loadingAnimation("Checking details", false);

    User *res = getUser(p.email, users);
    if(res != NULL) {
        printf("\n\nUser already exists! Please log in.");
        free(res);
    }

    else {
        loadingAnimation("Saving details.\nPlease wait", true);
        fwrite(&p, sizeof(User), 1, users);
        printf("Details saved successfully.\nPlease log in.\n\n");
    }
}







/*************************\
|   logging in the user   |
\*************************/
User* loginUser(FILE *users) {
    char email[50], password[25];
    bool leaveLogin = false;
    User *res;

    loadingAnimation("Loading sign in page", true);
    printSignInBanner();

    do {
        printf("\n\nEnter email : ");
        scanf("\n%s", email);
        printf("Enter password : ");
        scanf("\n%s", password);

        res = getUser(email, users);
        loadingAnimation("Checking details", false);

        if(res == NULL || strcmpi(res -> password, password) != 0) {
            char tryAgain;
            res = NULL;
            printf("\n\nWrong email or password!\n\nDo you want to try again? (Enter \'Y\') : ");
            scanf("\n%c", &tryAgain);
            if(tryAgain != 'Y' && tryAgain != 'y')
                leaveLogin = true;
        }
        else {
            loadingAnimation("Logging in\nPlease wait", true);
            res -> orderArrInd = getOrders(email);
            printf("\n\nWelcome back, %s", res -> username);
            leaveLogin = true;
        }
    } while(!leaveLogin);

    return res;
}






/**************************\
|  displays contact info   |
\**************************/ 
void displayContactInfo(FILE *contact) {
    char storeName[25], storeContact[15], label[30] = "Contact: ";

    fseek(contact, 0, SEEK_SET);
    fgets(storeName, 25, contact);
    fgets(storeContact, 15, contact);
    
    storeName[strlen(storeName) - 1] = '\0';
    storeContact[strlen(storeContact) - 1] = '\0';

    printBoxEdge();
    printBoxField("");
    printBoxField(storeName);
    printBoxField("");
    printBoxField(strcat(label, storeContact));
    printBoxField("");
    printBoxEdge();
}






// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------------------------------------------------------------------------- //


int main() {
    bool exitProgram = false;
    char choice = '0';
    User *loggedInUser = NULL;
    FILE *items = NULL, *users = NULL, *orders = NULL, *contact = NULL;

    items = fopen(".\\data\\items.txt", "r");
    users = fopen(".\\data\\users.txt", "a+");
    orders = fopen(".\\data\\orders.txt", "r");
    contact = fopen(".\\data\\contact.txt", "r");



    populateItemArr(items);
    populateOrderArr(orders);


    do {

        if(choice == '0') {
            loadingAnimation("Loading dashboard", true);
            printMainMenu(loggedInUser != NULL);
        }

        printf("\n\nEnter choice: ");
        scanf("\n%c", &choice);
        getc(stdin);

        switch(choice) {
            case '1': {
                if (items == NULL)
                    printf("\n\nERROR! Could not fetch data.\nPlease try again later!");
                else
                    displayItems(loggedInUser, orders);
                break;
            }

            case '2': {
                if(loggedInUser == NULL)
                    registerUser(users);
                else
                    viewCart(loggedInUser);
                break;
            }

            case '3': {
                if(loggedInUser == NULL)
                    loggedInUser = loginUser(users);
                else {
                    loadingAnimation("Signing out", true);
                    loggedInUser = NULL;
                    printf("\n\nSigned out successfully");
                }
                break;
            }

            case '4': {
                loadingAnimation("Loading contact information", true);
                if(contact == NULL || isFileEmpty(contact))
                    printf("Could not fetch information!");
                else
                    displayContactInfo(contact);
                break;
            }

            case '5': {
                loadingAnimation("Exiting portal", false);
                exitProgram = true;
                break;
            }

            default: printf("\nERROR : Invalid Choice! Enter a valid choice.");
        }

        if(choice >= '1' && choice <= '4') {
            choice = '0';
            backToDashboard();
        }

    } while(!exitProgram);

    if(items != NULL)
        fclose(items);
    if(users != NULL)
        fclose(users);
    if(orders != NULL)
        fclose(orders);
    if(contact != NULL)
        fclose(contact);

    free(itemArr);
    free(orderArr);

    return 0;
}