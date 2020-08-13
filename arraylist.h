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
        s[len-1] = 0;//Aqui hace la corta
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
    char line[65565];// temporal
    strcpy(line, linetest);//copia la linea del parametro al temporal
    int arrayIndexes = 0;// contador
    char ** myArray = malloc(arrayIndexes * sizeof(*myArray));// memoria dinamica para un matrice de caracteres
    char * token = strtok(line, delimiter);//funcion de delimitador(split) divide una linea con un delimitador
    while( token != NULL ) {
        arrayIndexes++;//aumento
        myArray = realloc(myArray, (arrayIndexes) * sizeof(*myArray));//aumento del memoria con realloc
        myArray[arrayIndexes-1] = malloc(bufferSize * sizeof(char));//memoria dinamica adentro del nuevo espacio reservada
        strcpy(myArray[arrayIndexes-1], choppy(token));//copia el elemento al arreglo
        token = strtok(NULL, delimiter);//corta de nuevo con el delimitador hasta que no hay para cortar
    }
    struct ArrayList temp;
    temp.elementSize = arrayIndexes;//Cantidad de elementos
    temp.bufferSize = bufferSize;//Un buffer para las lineas
    temp.elements = myArray;//los elementos
    return temp;//retorna el array
}