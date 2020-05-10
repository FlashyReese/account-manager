/*
 * @author Yao Chung Hu (FlashyReese)
 * @date 2020-03-30
 *
 */

#include <string.h>
#include <stdlib.h>

struct ArrayList{
    int elementSize;
    int bufferSize;
    char ** elements;
};

/*
 * Cortar '\n' a un arreglo de caracteres
 */
char *choppy(char *s){
    int len = strlen(s);
    if(s[len-1] == 10)
        s[len-1] = 0;
    return s;
}

/*
 * Separar una linea de ascii con varios elementos con un delimitador.
 * Ejemplo:
 * Delimiter: ':::'
 * linetest: 'elemento1:::elemento2:::elemento3'
 * Resultado: {"elemento1", "elemento2", "elemento3"}
 */
struct ArrayList split(char linetest[], char delimiter[], int bufferSize){
    char line[65565];
    strcpy(line, linetest);
    int arrayIndexes = 0;
    char ** myArray = malloc(arrayIndexes * sizeof(*myArray));
    char * token = strtok(line, delimiter);
    while( token != NULL ) {
        arrayIndexes++;
        myArray = realloc(myArray, (arrayIndexes) * sizeof(*myArray));
        myArray[arrayIndexes-1] = malloc(bufferSize * sizeof(char));
        strcpy(myArray[arrayIndexes-1], choppy(token));
        token = strtok(NULL, delimiter);
    }
    struct ArrayList temp;
    temp.elementSize = arrayIndexes;
    temp.bufferSize = bufferSize;
    temp.elements = myArray;
    return temp;
}
/* Deprecated

struct ArrayList addElement(struct ArrayList old, char line[]){
    old.elementSize = old.elementSize+1;
    old.elements = realloc(old.elements, (old.elementSize) * sizeof(*old.elements));
    old.elements[old.elementSize-1] = malloc(old.bufferSize * sizeof(char));
    strcpy(old.elements[old.elementSize-1], line);
    return old;
}*/
