#ifndef STORAGE_H
#define STORAGE_H

/**
@brief storage with search function
An interface for different approaches of data storage (hash table, black-red tree)
*/
typedef struct storage_t{
	int (*add)  (void* self, const void* key, const void* element);
	void (*drop) (void* self, const void* key);
	void* (*find) (const void* self, const void* key);
	int (*clear)(void* self);
	int (*delete)(void* self);
} storage_t;


storage_t* RBTree_ctor(size_t element_size);
storage_t* init_hash(size_t element_size, size_t table_hash_size, void* hash_function);

size_t HashAllData(void* Data);


#endif // STORAGE_H
