/*
 * @author Yao Chung Hu (FlashyReese)
 * @date 2020-03-30
 *
 */

#include "arraylist.h"

struct ArrayList getLines(char path[]){
    struct ArrayList temp;
    temp.bufferSize = 2048;
    FILE *file = fopen(path, "r");
    int arrayIndexes = 0;
    char ** myArray = malloc(arrayIndexes * sizeof(*myArray));
    if(file != NULL){
        char line[2048];
        while(fgets(line, sizeof line, file) != NULL){
            arrayIndexes++;
            myArray = realloc(myArray, (arrayIndexes) * sizeof(*myArray));
            myArray[arrayIndexes-1] = malloc(2048 * sizeof(char));
            strcpy(myArray[arrayIndexes-1], choppy(line));
        }
        fclose (file);
    }else{
        printf("An Error occur while trying to open '%s'.\n", path);
        exit(EXIT_FAILURE);
    }
    temp.elementSize = arrayIndexes;
    temp.elements = myArray;
    return temp;
}

void test(){
    FILE *fin;
    FILE *fout;
    int character;
    fin=fopen("D:\\Projects\\CLionProjects\\Projecto Final\\in.txt", "r");
    fout=fopen("D:\\Projects\\CLionProjects\\Projecto Final\\out.txt","w");
    while((character=fgetc(fin))!=EOF){
        putchar(character); // It displays the right character (UTF8) in the terminal
        fprintf(fout,"%c ",character); // It displays weird characters in the file
    }
    fclose(fin);
    fclose(fout);
    printf("\nFile has been created...\n");
}