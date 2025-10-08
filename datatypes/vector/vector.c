#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

static const int DEFAULT_CAPACITY = 4;

typedef struct vector_t {
    int* elements;
    int capacity;
    int size;
} vector_t;

vector_t* vector_new() {
    return (vector_t*) malloc(sizeof(vector_t));
}

void vector_ctr(vector_t* obj) {
    // sizeof(int) figures out whether int is 32 or 64 bit
    obj->elements = calloc(DEFAULT_CAPACITY, sizeof(int));
    obj->capacity = DEFAULT_CAPACITY;
    obj->size = 0;
}

void vector_dtr(vector_t* obj) {
    free(obj->elements);
    free(obj);
}

int vector_size(vector_t* obj) {
    return obj->size;
}

int vector_capacity(vector_t* obj) {
    return obj->capacity;
}

void vector_push_back(struct vector_t* obj, const int element) {
    int size = obj->size;
    int capacity = obj->capacity;

    // if vector is full
    if(size > 0 && size % capacity == 0) {
        // Increase in Steps of DEFAULT_CAPACITY
        obj->capacity = (size / DEFAULT_CAPACITY + 1) * DEFAULT_CAPACITY;
        obj->elements = realloc(obj->elements, obj->capacity * sizeof(int));
        printf("New allocation: %d\n", obj->capacity);
    }

    obj->elements[obj->size++] = element;
}

int vector_at(vector_t* obj, const int index)
{
    return obj->elements[index];
}

int* vector_begin(vector_t* obj)
{
    return &obj->elements[0];
}

int* vector_end(vector_t* obj)
{
    /* Returns a THEORETICAL past-the-end Element 
        and thus shall not be dereferenced! */
    return &obj->elements[obj->size];
}

void vector_pop_back(vector_t* obj)
{
    int lastIndex = obj->size - 1;
    obj->elements[lastIndex] = 0;
    obj->size--;
}

void vector_erase(vector_t* obj, int* first, int* last)
{
    int lastElement = (obj->size - 1);
    int firstElement = first - &obj->elements[0];

    for(int i=firstElement; i < lastElement; i++) {
        obj->elements[i] = obj->elements[i+1];
    }
    obj->size -= (last - first) > 0 ? (last - first) : 1;
}