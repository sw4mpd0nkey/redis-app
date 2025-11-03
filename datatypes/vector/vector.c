#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

static const int DEFAULT_CAPACITY = 4;


vector *create_vector(int word_size, int (*compare)(const void *, const void *)) {
    vector *newboi = (vector *)calloc(1, sizeof(vector));
    newboi->cmp = compare;
    newboi->size = 0;
    newboi->capacity = DEFAULT_CAPACITY;
    newboi->word_size = word_size;
    newboi->elements = calloc(DEFAULT_CAPACITY, word_size);

    return newboi;
}

unsigned int get_size(vector *vector) {
    return vector->size;
}

unsigned int get_capacity(vector *vector) {
    return vector->capacity;
}


int is_empty(vector *vector) {
    if(vector->size > 0) {
        return 1;
    } else {
        return 0;
    }
}


void push_back(vector *vector, void *data) {

    // verify that there is space in the vector
    if (size(vector) < capacity(vector)) {

        // no need to resize so set a pointer to the elements attribute
        char *ptr = (char *)vector->elements;
        // move the pointer to the end of the vector 
        ptr += (size(vector) * (vector->word_size));
        // create a copy of the data and tack that onto the vector
        memcpy((void *)ptr, (const void *)data, vector->word_size);
        // increase the size value
        vector->size++;
    } else {
        // needs to be resized so we create a tmp pointer to the current data
        void *tmp = vector->elements;
        // set the vectors elements to a new bigger size
        vector->elements = calloc(2 * capacity(vector), vector->word_size);
        // increase capacity size
        vector->capacity = 2 * capacity(vector);
        // add new tmp data section to the vectors elements
        memcpy(vector->elements, tmp, size(vector) * vector->word_size);
        // clear the data at tmp
        free(tmp);
        // call push_back again with the same data and it'll be added to the vector
        push_back(vector, data);
    }
}

void pop_back(vector *vec) {
    if (size(vec) != 0) {
        vec->size -= 1;
    }
}

void delete_vector(vector *vector) {
    if (vector->elements != NULL) {
        free(vector->elements);
        vector->elements = NULL;
    }
}

void *get(vector *vector, unsigned int index) {
    return (void *)((char *)vector->elements + (index * vector->word_size));
}

void *set(vector *vector, void *data, unsigned int index) {
    if (index < capacity(vector)) {
        void *ptr = (void *)((char *)vector->elements + (index * (vector->word_size)));
        memcpy(ptr, (const void *)data, vector->word_size);
    
        if (index >= vector->size) {
            vector->size = (index + 1);
        }

        return ptr;
    }
  
    return NULL;
}

void shrinkToFit(vector *vector) {
  if (size(vector) != 0) {
    void *tmp = vector->elements;
    vector->elements = calloc(vector->size, vector->word_size);
    memcpy(vector->elements, tmp, size(vector) * vector->word_size);
    free(tmp);
    vector->capacity = vector->size;
  }
}

void insert(vector *vector, void *elemAddr, unsigned int position) {

  unsigned int len = size(vector);
  
  if (len == position) {
    push_back(vector, elemAddr);
    return;
  
} else {

    push_back(vector, (char *)vector->elements + (len - 1) * vector->word_size);
    memmove((void *)((char *)vector->elements + (position + 1) * vector->word_size),
            (void *)((char *)vector->elements + position * vector->word_size),
            (len - position - 1) * vector->word_size);
    set(vector, elemAddr, position);
    return;
  }
}

void removeAt(vector *vector, unsigned int position) {

  int len = size(vector);
  if (position == len - 1) {
    pop_back(vector);
  } else {
    for (unsigned int i = position + 1; i < size(vector); i++) {
      set(vector, get(vector, i), i - 1);
    }
    (vector->size)--;
  }
}

void printAll(vector *vector, void (*printFunc)(const void *),
              char printNewLineAtTheEnd) {
  for (unsigned int i = 0; i < size(vector); i++) {
    printFunc(get(vector, i));
  }
  if (printNewLineAtTheEnd) {
    printf("\n");
  }
}

// uses the built in qsort to sort the generic vector
void sort(vector *vector) { 
    qsort(vector->elements, size(vector), vector->word_size, vector->cmp); 
}