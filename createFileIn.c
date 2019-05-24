#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 31

int main(){
	FILE *in;
	char outString[STRING_SIZE+1];

	if ((in = fopen("./in", "w+t")) == NULL) {
        printf("File can't be opened!\n");
        exit(0);
    }

	for (int i = 0; i < 250000; i++) {
	    int last = STRING_SIZE - 1;
        for (int i = 0; i < last; i++) {
            outString[i] = (char)' ' + (rand() % ('z' - ' ' + 1));
        }
        outString[STRING_SIZE-1] = '\n';
        outString[STRING_SIZE] = '\0';
        fputs(outString, in);
        fputs(outString, in);
    }

	fclose(in);
	return 0;
}
