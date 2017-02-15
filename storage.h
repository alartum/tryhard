#ifndef STORAGE_H
#define STORAGE_H

/**
@brief storage with search function
An interface for different approaches of data storage (hash table, black-red tree)
*/
typedef struct storage_t{
    void* manager; /**< The data structure responsible for storing and operating the data. */

    size_t size;
    
    int   (*add)  (void* self, const void* element);
    void  (*drop) (void* self, const void* element);
    void* (*find) (const void* self, const void* element);
    int   (*clear)(void* self, const void* element)
    int   (*delete)(void* self);
} storage_t;

storage_t* RBTree_ctor(size_t element_size);
storage_t* init_hash();

#endif // STORAGE_H
