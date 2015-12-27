/**
 * Test hash table
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
#include "ptscheme.h"
#include "memmanager.h"

namelist *results;
object *obj1
object *obj2

//TODO: Add back in hash table test script

int main(void) {

/*
    printf("testing hashtable.c ...\n");

    printf("allocating test object 1...");
    obj1 = alloc_object();
    obj1->type = SYMBOL;
    obj1->data.symbol.value = malloc(strlen("TEST 1") + 1);
    if (obj1->data.symbol.value == NULL) {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }
    printf("success\n");
    strcpy(obj->data.symbol.value, "TEST 1");

    printf("allocating test object 2...");
    obj2 = alloc_object();
    obj2->type = SYMBOL;
    obj2->data.symbol.value = malloc(strlen("TEST 2") + 1);
    if (obj2->data.symbol.value == NULL) {
        printf("failed\n");
        exit(EXIT_FAILURE);
    }
    printf("success\n");
    strcpy(obj2->data.symbol.value, "TEST 2");

    printf("\tcreating entry with key 'A'...");
    results = hashadd("A", obj1);
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
*/
}
