#include "vector.h"


int main()
{
    vector_t* my_vector = vector_new();
    vector_ctr(my_vector);

    printf("Vector Size: %d\n", vector_size(my_vector));
    printf("Vector Capacity: %d\n", vector_capacity(my_vector));

    vector_push_back(my_vector, 42);
    printf("pushing value: %d\n", 42);
    printf("Vector Size: %d\n", vector_size(my_vector));
    printf("Vector Capacity: %d\n", vector_capacity(my_vector));

    vector_push_back(my_vector, 55);
    printf("pushing value: %d\n", 55);

    vector_push_back(my_vector, 34);
    printf("pushing value: %d\n", 34);
    vector_push_back(my_vector, 67);
    printf("pushing value: %d\n", 67);
    vector_push_back(my_vector, 70);
    printf("pushing value: %d\n", 70);

    printf("Vector Size: %d\n", vector_size(my_vector));
    printf("Vector Capacity: %d\n", vector_capacity(my_vector));


    printf("Vector Element begin(): %d\n", *vector_begin(my_vector));
    /* printf("Vector Element end(): %d\n", *vector_end(my_vector));  /* NEVER! do this */

    for(int i = 0; i<5; i++) {
        printf("Element at %d: %d\n", i, vector_at(my_vector, i));
    }

    for(int* it=vector_begin(my_vector); it != vector_end(my_vector); it++) {
        printf("Element iterated: %d\n", *it);
    }

    vector_dtr(my_vector);

    return 0;
}