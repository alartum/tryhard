#ifndef STORAGE_H
#define STORAGE_H

/**
@brief storage with search function
An interface for different approaches of data storage (hash table, black-red tree)
*/
typedef struct storage_t{
    void* manager; /**< The data structure responsible for storing and operating the data. */

    void (*add)(void* self, const void* element);
    void (*drop)(void* self, const void* element);
    void* (*find)(const void* self, const void* element);
    void (*delete)(void* self);
} storage_t;

storage_t* RBTree_ctor();

#endif // STORAGE_H
