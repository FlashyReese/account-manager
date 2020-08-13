#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "accounts.h"
#include "cipher.h"
/*
 * @authors: Miguel Humberto Herrera Gonzalez, Yao Chung Hu
 * @version: 1.2
 * @date 2020-03-30
 */

static char filePathAccounts[] = "accounts.db";//"D:\\Projects\\CLionProjects\\Projecto Final\\accounts.db"; //Todo: Favor de cambiar esto dependiendo donde esta guardada las cuentas
static char filePathCipher[] = "cipher.key";//"D:\\Projects\\CLionProjects\\Projecto Final\\cipher.key"; //Todo: Favor de cambiar esto dependiendo donde esta guardada la llave
static char delimiter[] = ":::::";
static int bufferSize = 2048;
static struct Accounts savedAccounts;

void mainMenuDisplay(int error);
void cipherKeyDisplay(int error);
void viewAccountsDisplay(int error);
void viewAccountsAllDisplay(int error, int mode);


char * getCurrentCipherKey(){
    char existingLine[2048];
    FILE *file = fopen(filePathCipher, "rb");// Abrir el archivo donde esta guardada el llave
    if(file == NULL) //Si no existe el archivo crearlo
    {
        file = fopen(filePathCipher, "wb");
        file = fopen(filePathCipher, "rb");
    }
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
    return existingLine; //retorna el llave del archivo
}

void viewAccountsAllInput(int mode){
    int option;
    scanf("%d", &option);
    if(mode == 0){
        if(option > 0 && option <= savedAccounts.elementSize){
            struct Account temp = savedAccounts.elements[option-1];
            printf("Site: %s\n", temp.site);
            printf("Username/Email: %s\n", temp.name);
            printf("Password: encrypted(%s), decrypted(%s)\n", temp.password, decrypt(temp.password, getCurrentCipherKey()));//Solo muestra y desencripta
        }
    }else if(mode == 1){//Change Password
        if(option > 0 && option <= savedAccounts.elementSize){
            getchar();
            struct Account temp = savedAccounts.elements[option-1];
            printf("Site: %s\n", temp.site);
            printf("Username/Email: %s\n", temp.name);
            printf("Please enter your old password:\n");
            char oldpass[2048], pass[2048], confirmpass[2048];
            fgets(oldpass, 2048, stdin);
            if(strcmp(choppy(oldpass), decrypt(temp.password, getCurrentCipherKey())) == 0){
                printf("Please enter your new password:\n");
                fgets(pass, 2048, stdin);
                printf("Please confirm your new password:\n");
                fgets(confirmpass, 2048, stdin);
                if(strcmp(choppy(confirmpass), choppy(pass)) == 0){
                    savedAccounts.elements[option-1].password = encrypt(choppy(pass), getCurrentCipherKey());// Cambia la contrasena
                    write(savedAccounts, filePathAccounts, delimiter);// guarda la lista
                }else{
                    printf("New Passwords did not match!\n");
                }
            }else{
                printf("Old Password did not match!\n");
            }
        }
    }else if(mode == 2){//Deletion
        if(option > 0 && option <= savedAccounts.elementSize){
            struct Account temp = savedAccounts.elements[option-1];
            printf("Site: %s\n", temp.site);
            printf("Username/Email: %s\n", temp.name);
            printf("Are you sure you want to remove this account?\n1. Yes\n2. No\n");
            int confirm;
            scanf("%d", &confirm);
            if(confirm == 1){
                savedAccounts = removeAccount(savedAccounts, temp);//Elimina una cuenta de una lista
                write(savedAccounts, filePathAccounts, delimiter);//Guarda lista
            }
        }
    }
}

void viewAccountsAllDisplay(int error, int mode){
    printf("---------------------------------------------------------\n");
    printf("---------------------Account Manager---------------------\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < savedAccounts.elementSize; i++) {
        printf("%d. %s - %s\n", i+1, savedAccounts.elements[i].site, savedAccounts.elements[i].name);
    }
    viewAccountsAllInput(mode);
}

void searchAccountBySite(){
    printf("Please enter a site:(Example. uady.mx)\n");
    char line[255];
    scanf("%s", &line);
    struct Account temp;
    int found = 0;
    for (int i = 0; i < savedAccounts.elementSize; i++) {
        if(strcmp(savedAccounts.elements[i].site, line) == 0){//Busqueda por sitio
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
        viewAccountsAllDisplay(0, 0);
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
            savedAccounts.elements[i].password = encrypt(decrypt(savedAccounts.elements[i].password, getCurrentCipherKey()), cipherKey);// Cambia todo las cuentas encriptado con el viejo llave con uno nuevo
        }
        write(savedAccounts, filePathAccounts, delimiter);// guarda lista nuevamente encriptado
    }
    FILE *fptr;
    fptr = fopen(filePathCipher,"wb");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr, "%s", cipherKey);//guarda la llave
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
    int existing = strlen(getCurrentCipherKey()) == 95 ? 1 : 0;
    if(existing){
        printf("Looks like you already have an existing saved cipher key(%s). Would you like to replace it with a newly generated one?\n", getCurrentCipherKey());
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
    char * key = getCurrentCipherKey();
    if(strlen(key) != 95){
        printf("Please generate a key first!!!\n");
        mainMenuDisplay(0);
    }
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
        savedAccounts = addAccount(savedAccounts, temp);// Guarda una nueva cuenta a la lista
        write(savedAccounts, filePathAccounts, delimiter); // Guarda la lista
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
            viewAccountsAllDisplay(0, 1);
            mainMenuDisplay(0);
            break;
        case 4:
            viewAccountsAllDisplay(0, 2);
            mainMenuDisplay(0);
            break;
        case 5:
            cipherKeyDisplay(0);
            break;
        case 6:

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
    printf("3. Modify Password of Existing Account\n");
    printf("4. Remove Account\n");
    printf("5. Generate Cipher Key\n");
    printf("6. Quit Application\n");
    if(error == 1){
        printf("Invalid Input, Please Try Again!\n");
    }
    mainMenuInput();
}

int main(int argc, char **argv) {
    if (setlocale(LC_ALL, "es") == NULL) {
        puts("Unable to set locale");
    }
    srand(time(NULL));//Usado para un randomizador
    savedAccounts = getAccounts(filePathAccounts, delimiter, bufferSize);//Carga todas las cuentas guardadas
    mainMenuDisplay(0);
    return 0;
}
