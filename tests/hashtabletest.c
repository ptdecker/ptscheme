/**
 * Test hash table
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

namelist *results;

int main(void) {
    printf("testing hashtable.c ...\n");

    printf("\tcreating entry with key 'A'...");
    results = hashput("A","data for A");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "A") == 0 && strcmp(results->value, "data for A") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tcreating entry with key 'B'...");
    results = hashput("B","data for B");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "B") == 0 && strcmp(results->value, "data for B") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tattempting to retreive key 'A'...");
    results = hashget("A");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "A") == 0 && strcmp(results->value, "data for A") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tattempting to retrieve key 'B'...");
    results = hashget("B");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "B") == 0 && strcmp(results->value, "data for B") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tattempting to change key 'B'...");
    results = hashput("B", "new data for B");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "B") == 0 && strcmp(results->value, "new data for B") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tattempting to retrieve updated key 'B'...");
    results = hashget("B");
    printf("(key: %s, value: %s)...", results->key, results->value);
    if (results != NULL && strcmp(results->key, "B") == 0 && strcmp(results->value, "new data for B") == 0)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\tattempting to retrieve non-existant key 'C'...");
    results = hashget("C");
    if (results == NULL)
        printf("success\n");
    else {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
