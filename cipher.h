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

char alpha[95] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

int containsChar(char * string, char c, int size){
    for (int i = 0; i < size; i++) {
        if(string[i] == c){
            return 1;
        }
    }
    return 0;
}

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

struct Keys getKeys(char cipherKey[]){
    int length = strlen(alpha);
    struct Key * keys = malloc(length * sizeof(struct Key));
    for (int i = 0; i < length; i++) {
        keys[i].original = alpha[i];
        keys[i].placeholder = cipherKey[i];
    }
    struct Keys temp;
    temp.elementSize = length;
    temp.elements = keys;
    return temp;
}

char * encrypt(char string[], char cipherKey[]){
    char * newString = malloc( strlen(string)+1 * sizeof(char));
    struct Keys cipherer = getKeys(cipherKey);
    for (int i = 0; i <  strlen(string); i++) {
        for (int j = 0; j < cipherer.elementSize; j++) {
            if(string[i] == cipherer.elements[j].original){
                newString[i] = cipherer.elements[j].placeholder;
            }
        }
    }
    newString[strlen(string)] = '\0';
    return newString;
}

char * decrypt(char string[], char cipherKey[]){
    char * newString = malloc( strlen(string)+1 * sizeof(char));
    struct Keys cipherer = getKeys(cipherKey);
    for (int i = 0; i <  strlen(string); i++) {
        for (int j = 0; j < cipherer.elementSize; j++) {
            if(string[i] == cipherer.elements[j].placeholder){
                newString[i] = cipherer.elements[j].original;
            }
        }
    }
    newString[strlen(string)] = '\0';
    return newString;
}

