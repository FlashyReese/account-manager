#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "accounts.h"
#include "cipher.h"

static char filePathAccounts[] = "D:\\Projects\\CLionProjects\\Projecto Final\\accounts.db"; //Todo: Favor de cambiar esto dependiendo donde esta guardada las cuentas
static char filePathCipher[] = "D:\\Projects\\CLionProjects\\Projecto Final\\cipher.key"; //Todo: Favor de cambiar esto dependiendo donde esta guardada la llave
static char delimiter[] = ":::::";
static int bufferSize = 2048;
static struct Accounts savedAccounts;

void mainMenuDisplay(int error);
void cipherKeyDisplay(int error);
void viewAccountsDisplay(int error);
void viewAccountsAllDisplay(int error);


char * getCurrentCipherKey(){
    char existingLine[2048];
    FILE *file = fopen(filePathCipher, "r");
    if(file != NULL){
        char line[2048];
        while(fgets(line, sizeof line, file) != NULL){
            strcpy(existingLine, choppy(line));
            break;
        }
        fclose (file);
    }else{
        printf("An Error occur while trying to open '%s'.\n", filePathCipher);
        exit(EXIT_FAILURE);
    }
    int existing = strlen(existingLine) >= 94 ? 1 : 0;
    if(existing == 0){
        printf("Please generate a cipher key first!!!\n");
        mainMenuDisplay(0);
    }
    return existingLine;
}

void viewAccountsAllInput(){
    int option;
    scanf("%d", &option);
    if(option > 0 && option <= savedAccounts.elementSize){
        struct Account temp = savedAccounts.elements[option-1];
        printf("Site: %s\n", temp.site);
        printf("Username/Email: %s\n", temp.name);
        printf("Password: encrypted(%s), decrypted(%s)\n", temp.password, decrypt(temp.password, getCurrentCipherKey()));
    }
}

void viewAccountsAllDisplay(int error){
    printf("---------------------------------------------------------\n");
    printf("---------------------Account Manager---------------------\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < savedAccounts.elementSize; i++) {
        printf("%d. %s\n", i+1, savedAccounts.elements[i].site);
    }
    viewAccountsAllInput();
}

void searchAccountBySite(){
    printf("Please enter a site:(Example. uady.mx)\n");
    char line[255];
    scanf("%s", &line);
    struct Account temp;
    int found = 0;
    for (int i = 0; i < savedAccounts.elementSize; i++) {
        if(strcmp(savedAccounts.elements[i].site, line) == 0){
            temp = savedAccounts.elements[i];
            found = 1;
            break;
        }
    }
    if(found == 1){
        printf("Site: %s\n", temp.site);
        printf("Username/Email: %s\n", temp.name);
        printf("Password: encrypted(%s), decrypted(%s)\n", temp.password, decrypt(temp.password, getCurrentCipherKey()));
    }else{
        printf("Nothing found by %s\n", line);
    }
}

void viewAccountsInput(){
    int option;
    scanf("%d", &option);
    if(option == 1){
        searchAccountBySite();
        viewAccountsDisplay(0);
    }else if(option == 2){
        viewAccountsAllDisplay(0);
        viewAccountsDisplay(0);
    }else if(option == 3){
        mainMenuDisplay(0);
    }else{
        viewAccountsDisplay(1);
    }
}


void viewAccountsDisplay(int error){
    printf("---------------------------------------------------------\n");
    printf("---------------------Account Manager---------------------\n");
    printf("---------------------------------------------------------\n");
    printf("1. Search with site name\n");
    printf("2. Browse all accounts\n");
    printf("3. Previous Page\n");
    if(error == 1){
        printf("Invalid Input, Please Try Again!\n");
    }
    viewAccountsInput();
}

void generateKey(int mode){
    char * cipherKey = generateCipherKey();
    printf("Newly Generated Cipher Key: %s\n", cipherKey);
    if(mode == 0){
        for (int i = 0; i < savedAccounts.elementSize; i++) {
            savedAccounts.elements[i].password = encrypt(decrypt(savedAccounts.elements[i].password, getCurrentCipherKey()), cipherKey);
        }
    }
    FILE *fptr;
    fptr = fopen(filePathCipher,"w");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr, "%s", cipherKey);
    fclose(fptr);
}

void cipherKeyInput(){
    int option;
    scanf("%d", &option);
    if(option == 1){
        mainMenuDisplay(0);
    }else if(option == 2){
        generateKey(0);
        mainMenuDisplay(0);
    }else{
        cipherKeyDisplay(1);
    }
}

void cipherKeyDisplay(int error){
    char existingLine[2048];
    FILE *file = fopen(filePathCipher, "r");
    if(file != NULL){
        char line[2048];
        while(fgets(line, sizeof line, file) != NULL){
            strcpy(existingLine, choppy(line));
            break;
        }
        fclose (file);
    }else{
        printf("An Error occur while trying to open '%s'.\n", filePathCipher);
        exit(EXIT_FAILURE);
    }
    int existing = strlen(existingLine) >= 94 ? 1 : 0;
    if(existing){
        printf("Looks like you already have an existing saved cipher key(%s). Would you like to replace it with a newly generated one?\n", existingLine);
        printf("WARNING: Any existing saved accounts encrypted with an old cipher key may be lost!!!\nProceed with precaution.\n");
        printf("1. Continue using existing cipher key\n");
        printf("2. Replace old cipher key\n");
        if(error == 1){
            printf("Invalid Input, Please Try Again!\n");
        }
        cipherKeyInput();
    }else{
        generateKey(1);
        mainMenuDisplay(0);
    }
}


void saveNewAccount(){
    fflush(stdin);
    fflush(stdout);
    getchar();
    printf("---------------------------------------------------------\n");
    printf("---------------------Account Manager---------------------\n");
    printf("---------------------------------------------------------\n");
    char site[2048], name[2048], password[2048], confirmpass[2048];
    printf("Site:\n");
    fgets(site, 2048, stdin);
    printf("Username/Email:\n");
    fgets(name, 2048, stdin);
    printf("Password:\n");
    fgets(password, 2048, stdin);
    printf("Password:\n");
    fgets(confirmpass, 2048, stdin);
    if(strcmp(password, confirmpass) == 0){
        struct Account temp = {choppy(site),choppy(name),encrypt(choppy(password), getCurrentCipherKey())};
        savedAccounts = addAccount(savedAccounts, temp);
        write(savedAccounts, filePathAccounts, delimiter);
    }else{
        printf("Passwords did not match!\n");
    }
    mainMenuDisplay(0);
}


void mainMenuInput(){
    int option;
    scanf("%d", &option);
    switch(option){
        case 1:
            viewAccountsDisplay(0);
            break;
        case 2:
            saveNewAccount();
            break;
        case 3:

            break;
        case 4:
            cipherKeyDisplay(0);
            break;
        case 5:

            break;
        default:
            mainMenuDisplay(1);
            break;
    }
}
void mainMenuDisplay(int error){
    printf("---------------------------------------------------------\n");
    printf("---------------------Account Manager---------------------\n");
    printf("---------------------------------------------------------\n");
    printf("1. View Accounts\n");
    printf("2. Save New Account\n");
    printf("3. Modify Existing Account\n");
    printf("4. Generate Cipher Key\n");
    printf("5. Quit Application\n");
    if(error == 1){
        printf("Invalid Input, Please Try Again!\n");
    }
    mainMenuInput();
}

int main(int argc, char **argv) {
    if (setlocale(LC_ALL, "es") == NULL) {
        puts("Unable to set locale");
    }
    srand(time(NULL));
    savedAccounts = getAccounts(filePathAccounts, delimiter, bufferSize);

    mainMenuDisplay(0);
    /*char * cipherKey = generateCipherKey();
    printf("Llave generado: %s\n", cipherKey);
    char * encrypted = encrypt("Esto es un texto no encriptado", cipherKey);
    printf("Texto encriptado: %s\n", encrypted);
    char * decrypted = decrypt(encrypted, cipherKey);
    printf("Texto desencriptado: %s\n\n", decrypted);
    free(cipherKey);

    //struct Accounts list = getAccounts(argv[1], argv[2], 2048);
    printf("Listado en el .txt guardado:\n");
    struct Accounts list = getAccounts(filePath, delimiter, bufferSize);
    for (int i = 0; i < list.elementSize; ++i) {
        printf("Site: %s Name: %s Password: %s\n", list.elements[i].site, list.elements[i].name, list.elements[i].password);
    }
    printf("\n");
    struct Account test = {"facebook.com","usuario1","qwerty"};
    list = addAccount(list, test);
    struct Account test2 = {"google.com","usuario2","asdf"};
    list = addAccount(list, test2);
    printf("Listado con 2 nuevos cuentas:\n");
    for (int i = 0; i < list.elementSize; ++i) {
        printf("Site: %s Name: %s Password: %s\n", list.elements[i].site, list.elements[i].name, list.elements[i].password);
    }
    printf("\n");
    *//*list = removeAccount(list, test);
    printf("Listado con uno de los nuevos cuentas eliminado:\n");
    for (int i = 0; i < list.elementSize; ++i) {
        printf("Site: %s Name: %s Password: %s\n", list.elements[i].site, list.elements[i].name, list.elements[i].password);
    }*//*
    write(list, "D:\\Projects\\CLionProjects\\Projecto Final\\accounts.db", "::");*/
    return 0;
}
