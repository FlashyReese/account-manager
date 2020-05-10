/*
 * @author Yao Chung Hu (FlashyReese)
 * @date 2020-03-30
 *
 */
#include "file.h"

struct Account{
    char * site;
    char * name;
    char * password;
};

struct Accounts{
    int bufferSize;
    int elementSize;
    struct Account * elements;
};

/*
 * Leer cuentas guardadas con el path y el delimitador.
 */
struct Accounts getAccounts(char filePath[], char delimiter[], int bufferSize){
    struct ArrayList accountsList = getLines(filePath);
    struct Account * accounts = malloc(accountsList.elementSize * sizeof(struct Account));
    for (int i = 0; i < accountsList.elementSize; ++i) {
        struct ArrayList splitter = split(accountsList.elements[i], delimiter, bufferSize);
        if(splitter.elementSize != 3){
            printf("Invalid line at '%d'", i+1);
            exit(EXIT_FAILURE);
        }
        accounts[i].site = malloc(accountsList.bufferSize * sizeof(char));
        strcpy(accounts[i].site, splitter.elements[0]);
        accounts[i].name = malloc(accountsList.bufferSize * sizeof(char));
        strcpy(accounts[i].name, splitter.elements[1]);
        accounts[i].password = malloc(accountsList.bufferSize * sizeof(char));
        strcpy(accounts[i].password, splitter.elements[2]);
    }
    struct Accounts temp;
    temp.bufferSize = accountsList.bufferSize;
    temp.elementSize = accountsList.elementSize;
    temp.elements = accounts;
    return temp;
}

/*
 * Agregar una cuenta a la estructura de cuentas.
 */
struct Accounts addAccount(struct Accounts accounts, struct Account account){
    accounts.elementSize++;
    accounts.elements = realloc(accounts.elements, (accounts.elementSize) * sizeof(struct Account));
    accounts.elements[accounts.elementSize-1] = account;
    return accounts;
}

/*
 * Borrar una cuenta de la estructura de cuentas.
 */
struct Accounts removeAccount(struct Accounts accounts, struct Account account){
    int index = -1;
    for (int i = 0; i < accounts.elementSize; i++) {
        if(strcmp(accounts.elements[i].name , account.name) == 0 && strcmp(accounts.elements[i].site , account.site) == 0 && strcmp(accounts.elements[i].password , account.password) == 0){
            index = i;
        }
    }
    if(index == -1){
        printf("Account not found, could not be deleted!\n");
    }else{
        for (int i = index; i < accounts.elementSize - 1; i++) {
            accounts.elements[i] = accounts.elements[i+1];
        }
        accounts.elementSize--;
    }
    return accounts;
}

/*
 * Guardar estructura de cuentas a un path especifico, usando un delimitador.
 */
void write(struct Accounts accounts, char filePath[], char delimiter[]){
    FILE *fptr;
    fptr = fopen(filePath,"w");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    for (int i = 0; i < accounts.elementSize; ++i) {
        fprintf(fptr,"%s%s%s%s%s\n", accounts.elements[i].site, delimiter, accounts.elements[i].name, delimiter, accounts.elements[i].password);
    }
    fclose(fptr);
}
