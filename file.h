/*
 * @author Yao Chung Hu (FlashyReese)
 * @date 2020-03-30
 *
 */

#include "arraylist.h"

/*
 * Regresa una lista de lineas leidas de un archivo en forma estructura como una lista de elementos.
 */
struct ArrayList getLines(char path[]){
    struct ArrayList temp; // crea un estructura de arraylist temporal
    temp.bufferSize = 2048; // assigna su buffer de 2048 caracteres
    FILE *file = fopen(path, "rb"); // abrir archivo con path en modo lector
    if(file == NULL) //si el archivo no existe crearlo
    {
        file = fopen(path, "wb");
    }
    int arrayIndexes = 0;//contador de lineas
    char ** myArray = malloc(arrayIndexes * sizeof(*myArray)); // memoria dinamica con estructuras
    if(file != NULL){
        char line[2048]; // buffer
        while(fgets(line, sizeof line, file) != NULL){ // por cada linea en archivo
            arrayIndexes++;//aumento de lineas
            myArray = realloc(myArray, (arrayIndexes) * sizeof(*myArray));// realloc memoria
            myArray[arrayIndexes-1] = malloc(2048 * sizeof(char));//memoria dinamica en el subElemento
            strcpy(myArray[arrayIndexes-1], choppy(line));//assigna el linea leido al nuevo allocation
        }
        fclose (file);
    }else{
        printf("An Error occur while trying to open '%s'.\n", path);
        exit(EXIT_FAILURE);
    }
    temp.elementSize = arrayIndexes;//Cantidad de elementos osea lineas
    temp.elements = myArray;//Elementos
    return temp;
}