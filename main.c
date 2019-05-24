#include <stdio.h>
#include <stdlib.h>

#define FIRST_LEVEL_SIZE 256
#define INIT_SIZE 10
#define INCREASE_SIZE 1.1
#define STRING_SIZE 31

typedef struct element{
    char ch;
    unsigned short size;
    unsigned short current;
    struct element *next[];
} element;

element * createElement(unsigned short size) {
    struct element *newElement = (struct element *)malloc(sizeof(element) + sizeof(element*) * size);

    if (!newElement) {
        printf("Memory can't be allocated!");
        exit(0);
    }
    newElement->current = 0;
    newElement->size = size;

    return newElement;
}

element * endResizeElement(element *origElement) {
    unsigned short newSize = (unsigned short)((double)origElement->size * INCREASE_SIZE);
    struct element *newElement = (struct element *)realloc(
            origElement,
            sizeof(element) + sizeof(element*) * newSize);

    if (!newElement) {
        printf("Memory can't be allocated!");
        exit(0);
    }
    newElement->size = newSize;

    return newElement;
}

element * insertResizeElement(element *origElement, unsigned short insertPosition) {
    unsigned short newSize = (unsigned short)((double)origElement->size * INCREASE_SIZE);
    struct element *newElement = (struct element *)realloc(
            origElement,
            sizeof(element) + sizeof(element*) * newSize);

    if (!newElement) {
        printf("Memory can't be allocated!");
        exit(0);
    }

    newElement->size = newSize;
    for (int i = newElement->current; i > insertPosition; i--) {
        newElement->next[i] = newElement->next[i-1];
    }
    newElement->current++;

    return newElement;
}

element * createTail(char *inputString, unsigned short start) {
    if (inputString[start] == '\0') {
        return NULL;
    }
    element *newElement = createElement(INIT_SIZE);

    newElement->ch = inputString[start];
    newElement->next[0] = createTail(inputString, ++start);
    newElement->current++;

    return newElement;
}

element * parseString(char *inputString, unsigned short start, element *elementsStorage) {
    if (inputString[start] == '\0') {
        return elementsStorage;
    }

    for (int i = 0; i < elementsStorage->current; i++ ) {
        if (inputString[start] == elementsStorage->next[i]->ch) {
            elementsStorage->next[i] = parseString(inputString, start+1, elementsStorage->next[i]);
            return elementsStorage;
        }

        if (inputString[start] < elementsStorage->next[i]->ch) {
            element *tail = createTail(inputString, start);

            if (elementsStorage->current == elementsStorage->size) {
                element *newElement = insertResizeElement(elementsStorage, i);
                newElement->next[i] = tail;

                return newElement;
            } else {
                for (int j = elementsStorage->current; j > i; j--) {
                    elementsStorage->next[j] = elementsStorage->next[j-1];
                }
                elementsStorage->current++;
                elementsStorage->next[i] = tail;

                return elementsStorage;
            }
        }
    }

    element *tail = createTail(inputString, start);

    if (elementsStorage->current == elementsStorage->size) {
        element *newElement = endResizeElement(elementsStorage);
        newElement->next[newElement->current++] = tail;

        return newElement;
    } else {
        elementsStorage->next[elementsStorage->current++] = tail;
        return elementsStorage;
    }
}

void _printFile(char *stringOut, unsigned short start, element *elementStorage, FILE *out) {
    stringOut[start] = elementStorage->ch;

    if (elementStorage->next[0] == NULL) {
        stringOut[start+1] = '\0';
        fputs(stringOut, out);
        free(elementStorage->next[0]);
    } else {
        for (int i = 0; i < elementStorage->current; i++) {
            _printFile(stringOut, start+1, elementStorage->next[i], out);
            free(elementStorage->next[i]);
        }
    }
}

void printFile(element *elementsStorage, FILE *out) {
    char stringOut[STRING_SIZE+1];
    unsigned short start = 0;

    for (int i = 0; i < elementsStorage->current; i++) {
        _printFile(stringOut, start, elementsStorage->next[i], out);
        free(elementsStorage->next[i]);
    }
}


int main() {
    FILE *in, *out;
    element *elementsStorage = createElement(FIRST_LEVEL_SIZE);

    if ((in = fopen("../in", "r")) == NULL) {
        printf("File can't be opened!\n");
        exit(0);
    }

    while (!feof(in)) {
        char inputString[STRING_SIZE+1];

        if ( fgets(inputString, STRING_SIZE+1, in) != NULL) {
            elementsStorage = parseString(inputString, 0, elementsStorage);
        } else {
            break;
        }
    }

    if ((out = fopen("../out", "w")) == NULL) {
        printf("File can't be opened!\n");
        exit(0);
    }

    printFile(elementsStorage, out);

    fclose(in);
    fclose(out);
    return 0;
}
