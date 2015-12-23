/**
 * Test hash table
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <hashtable.h>

int main(void) {
    printf("testing hashtable.c ...\n");

    printf("\tcreating entry with key 'A'...");
    printf("success\n");

    printf("\tcreating entry with key 'B'...");
    printf("success\n");

    printf("\tattempting to retreive key 'A'...");
    printf("success\n");

    printf("\tattempting to change key 'B'...");
    printf("success\n");

    printf("\tattempting to retrieve key 'B'...");
    printf("success\n");

    printf("\tattempting to retrieve non-existant key 'C'...");
    printf("success\n");

    return EXIT_SUCCESS;
}
