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
    struct ArrayList accountsList = getLines(filePath);//Guarda lista de lineas de los cuentas
    struct Account * accounts = malloc(accountsList.elementSize * sizeof(struct Account));//mem dinamico de cuentas
    for (int i = 0; i < accountsList.elementSize; ++i) {// Para cada linea
        struct ArrayList splitter = split(accountsList.elements[i], delimiter, bufferSize);//separalo las lineas de nuevo con un delimitador
        /*
         * Ejemplo:
         * delimit: ":::::"
         * linea introducido: uady.mx:::::A12354678:::::qwerty
         * retorna: {"uady.mx", "A12345678", "qwerty"}
         * Ejemplo 2:
         * delimit: "@:@"
         * linea introducido: uady.mx@:@A12354678@:@qwerty
         * retorna: {"uady.mx", "A12345678", "qwerty"}
         */
        if(splitter.elementSize != 3){
            printf("Invalid line at '%d'", i+1);
            exit(EXIT_FAILURE);
        }
        accounts[i].site = malloc(accountsList.bufferSize * sizeof(char));//reserva nueva espacio para site
        strcpy(accounts[i].site, splitter.elements[0]);//posicion 0 osea "uady.mx"
        accounts[i].name = malloc(accountsList.bufferSize * sizeof(char));//reserva nueva espacio para name
        strcpy(accounts[i].name, splitter.elements[1]);//posicion 1 "A12345678"
        accounts[i].password = malloc(accountsList.bufferSize * sizeof(char));//reserva nueva espacio para password
        strcpy(accounts[i].password, splitter.elements[2]);//posicion 2 "qwerty"
    }
    struct Accounts temp;// temporal
    temp.bufferSize = accountsList.bufferSize;// Buffer
    temp.elementSize = accountsList.elementSize;// cantidad de elementos
    temp.elements = accounts;//elementos
    return temp;
}

/*
 * Agregar una cuenta a la estructura de cuentas.
 */
struct Accounts addAccount(struct Accounts accounts, struct Account account){
    accounts.elementSize++;//Aumenta la cantidad
    accounts.elements = realloc(accounts.elements, (accounts.elementSize) * sizeof(struct Account));// reserva una nueva espacio
    accounts.elements[accounts.elementSize-1] = account;//assigna el cuenta al nuevo espacio
    return accounts;//retorna nueva lista
}

/*
 * Borrar una cuenta de la estructura de cuentas.
 */
struct Accounts removeAccount(struct Accounts accounts, struct Account account){
    int index = -1;
    for (int i = 0; i < accounts.elementSize; i++) {
        if(strcmp(accounts.elements[i].name , account.name) == 0 && strcmp(accounts.elements[i].site , account.site) == 0 && strcmp(accounts.elements[i].password , account.password) == 0){//Confirma si el cuenta existe
            index = i;//guarda el indice de la cuenta
        }
    }
    if(index == -1){//Si no existe
        printf("Account not found, could not be deleted!\n");//Pues manda este mensaje
    }else{//Si existe
        for (int i = index; i < accounts.elementSize - 1; i++) {//Mueve toda lo que esta abajo del ese posicion un posicion hacia arriba
            accounts.elements[i] = accounts.elements[i+1];//Hace la operacion de mover
        }
        accounts.elementSize--;//decrementa un valor por eliminarlo
    }
    return accounts;//retorna nueva lista de cuentas
}

/*
 * Guardar estructura de cuentas a un path especifico, usando un delimitador.
 */
void write(struct Accounts accounts, char filePath[], char delimiter[]){
    FILE *fptr;
    fptr = fopen(filePath,"wb");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    for (int i = 0; i < accounts.elementSize; ++i) {
        fprintf(fptr,"%s%s%s%s%s\n", accounts.elements[i].site, delimiter, accounts.elements[i].name, delimiter, accounts.elements[i].password);//Guarda las cuentas con la lista introducida
    }
    fclose(fptr);
}
