#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Key{
    char original;
    char placeholder;
};

struct Keys{
    int elementSize;
    struct Key * elements;
};

char alpha[95] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";//Alphabeto para el randomizar, encriptar y desencriptar

/*
 * Checar si un arreglo de caracteres contiene un caracter.
 */
int containsChar(char * string, char c, int size){
    for (int i = 0; i < size; i++) {
        if(string[i] == c){
            return 1;
        }
    }
    return 0;
}
/*
 * Generar un llave random con el variable alpha
 */
char * generateCipherKey(){
    int size = 0;
    char * key = malloc(strlen(alpha)+1 * sizeof(char));
    while(size < strlen(alpha)){
        int randomIndex = (rand() % (strlen(alpha)));
        char randomChar = alpha[randomIndex];
        if(containsChar(key, randomChar, size) == 0){
            key[size++] = randomChar;
        }
    }
    key[size] = '\0';
    return key;
}

/*
 * Un espejo de alpha representada con el llave generado.
 */
struct Keys getKeys(char cipherKey[]){
    int length = strlen(alpha);
    struct Key * keys = malloc(length * sizeof(struct Key));
    for (int i = 0; i < length; i++) {
        keys[i].original = alpha[i];//El original alphabeto
        keys[i].placeholder = cipherKey[i];//El encriptado como mascara con el mismo posicion
    }
    struct Keys temp;
    temp.elementSize = length;
    temp.elements = keys;
    return temp;//retorna esa lista y alphabetos y su mascara como Llaves
}

/*
 * Encripta un arreglo de caracteres con una llave
 */
char * encrypt(char string[], char cipherKey[]){
    char * newString = malloc( strlen(string)+1 * sizeof(char));
    struct Keys cipherer = getKeys(cipherKey);//Agarrar esos llaves con el parametro
    for (int i = 0; i <  strlen(string); i++) {
        for (int j = 0; j < cipherer.elementSize; j++) {
            if(string[i] == cipherer.elements[j].original){//Por cada caracter igual al original
                newString[i] = cipherer.elements[j].placeholder;// Reemplazalo con el mascara
            }
        }
    }
    newString[strlen(string)] = '\0';// fin de linea
    return newString;//retorna el texto encriptado
}

/*
 * Decripta un arreglo de caracteres con una llave
 */
char * decrypt(char string[], char cipherKey[]){
    char * newString = malloc( strlen(string)+1 * sizeof(char));
    struct Keys cipherer = getKeys(cipherKey);//Agarrar esos llaves con el parametro
    for (int i = 0; i <  strlen(string); i++) {
        for (int j = 0; j < cipherer.elementSize; j++) {
            if(string[i] == cipherer.elements[j].placeholder){//Por cada caracter igual al mascara
                newString[i] = cipherer.elements[j].original;// Reemplazalo con el original
            }
        }
    }
    newString[strlen(string)] = '\0';// fin de linea
    return newString;//retorna el texto desencriptado
}

