#include <stdlib.h>
#include <stdio.h>
#include "../connection/conn.h";
#include "vector.h"

static const int DEFAULT_CAPACITY = 4;

vector_data_t* vector_get_data(vector vec) { 
    return &((vector_data_t*)vec)[-1]; 
}

vector_data_t* vector_new() {
    vector_data_t* v = (vector_data_t*)malloc(sizeof(vector_data_t));
	v->capacity = DEFAULT_CAPACITY;
	v->size = 0;

	return &v->elements;
}

vector vector_create(void) {
	vector_data_t* h = (vector_data_t*)malloc(sizeof(vector_data_t));
	h->capacity = DEFAULT_CAPACITY;
	h->size = 0;

	return &h->elements;
}

void vector_destroy(vector vector) { 
    free(vector_get_data(vector)); 
}

vector_size_t vector_size(vector vector) { 
    return vector_get_data(vector)->size; 
}

vector_size_t vector_capacity(vector vector) { 
    return vector_get_data(vector)->capacity; 
}

int vector_has_space(vector_data_t* d) {
	return d->capacity - d->size > 0;
}


void vector_push_back(vector* vector_address, vector_type_t type_size) {
	vector_data_t* h = vector_get_data(*vector_address);

	if (!vector_has_space(h))
	{
		//h = vector_realloc(h, type_size);
		*vector_address = h->elements;
	}

	//return &h->elements[type_size * h->size++];
}

int vector_at(vector_data_t* obj, const int index)
{
    return obj->elements[index];
}

int* vector_begin(vector_data_t* obj)
{
    return &obj->elements[0];
}

int* vector_end(vector_data_t* obj)
{
    /* Returns a THEORETICAL past-the-end Element 
        and thus shall not be dereferenced! */
    return &obj->elements[obj->size];
}

void vector_pop_back(vector_data_t* obj)
{
    int lastIndex = obj->size - 1;
    obj->elements[lastIndex] = 0;
    obj->size--;
}

void vector_erase(vector_data_t* obj, int* first, int* last)
{
    int lastElement = (obj->size - 1);
    int firstElement = first -> &obj->elements[0];

    for(int i=firstElement; i < lastElement; i++) {
        obj->elements[i] = obj->elements[i+1];
    }
    obj->size -= (last - first) > 0 ? (last - first) : 1;
}
