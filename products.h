/*
 * @author Yao Chung Hu (FlashyReese)
 * @date 2020-03-30
 *
 */
#include "file.h"

struct Product{
    char code128[48];
    char * name;
    int quantity;
    float price;
};

struct Products{
    int bufferSize;
    int elementSize;
    struct Product * elements;
};

struct Products getProducts(char filePath[], char delimiter[], int bufferSize){
    struct ArrayList productsList = getLines(filePath);
    struct Product *products = malloc(productsList.elementSize * sizeof(struct Product));
    for (int i = 0; i < productsList.elementSize; ++i) {
        struct ArrayList splitter = split(productsList.elements[i], delimiter, bufferSize);
        if(splitter.elementSize != 4){
            printf("Invalid line at '%d'", i+1);
            exit(EXIT_FAILURE);
        }
        strcpy(products[i].code128, splitter.elements[0]);
        products[i].name = malloc(productsList.bufferSize * sizeof(char));
        strcpy(products[i].name, splitter.elements[1]);
        products[i].quantity = atoi(splitter.elements[2]);
        products[i].price = atof(splitter.elements[3]);
    }
    struct Products temp;
    temp.bufferSize = productsList.bufferSize;
    temp.elementSize = productsList.elementSize;
    temp.elements = products;
    return temp;
}

struct Products addProduct(struct Products products, struct Product product){
    products.elementSize++;
    products.elements = realloc(products.elements, (products.elementSize) * sizeof(struct Product));
    products.elements[products.elementSize-1] = product;
    return products;
}