#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// vector type
struct vector
{
	void *elements;
	unsigned int size;
	unsigned int capacity;
	int word_size;
	int (*cmp)(const void *, const void *);
};
typedef struct vector vector;

// create a new vector
vector *create_vector(int word_size, int (*compare)(const void *, const void *));

// getter for size attribute
unsigned int get_size(vector *vector);

// getter for capacity attribute
unsigned int get_capacity(vector *vector);

// returns 1 for empty and 0 for not 
int is_empty(vector *vector);

// pushes new element onto end of the vector and resizes if needed
void push_back(vector *vector, void *data);

// pops the element off the end of the array
void pop_back(vector *vector);

// deletes the heap allocated memory
void delete_vector(vector *vector);

void *get(vector *vector, unsigned int index);

void *set(vector *vector, void *data, unsigned int index);

// shrinks the vector to the size of the vector. No unwanted storage used
void shrinkToFit(vector *vector);

// Insert an element at arbitrary position
void insert(vector *vector, void *elemAddr, unsigned int position);

// Delete an element from an arbitrary position
void removeAt(vector *vector, unsigned int position);

void print(vector *vector, void (*printFunc)(const void *), char printNewLineAtTheEnd);

// uses the built in qsort to sort the generic vector
void sort(vector *vector);

#endif // VECTOR_H