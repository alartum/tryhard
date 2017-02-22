#ifndef STORAGE_H
#define STORAGE_H

/**
@brief storage with search function
An interface for different approaches of data storage (hash table, black-red tree)
*/
#pragma pack(push, 8)
typedef struct storage_t{
	int (*add)  (void* self, const void* key, const void* element);
	void (*drop) (void* self, const void* key);
	void* (*find) (const void* self, const void* key);
	int (*clear)(void* self);
	int (*delete)(void* self);
	size_t (*size)();
} storage_t;
#pragma(pop)


storage_t* RBTree_ctor(size_t element_size);

/*
Function for init storage_t working on hash table.
element_size - is size of the save element in sizeof(char);
table_hash_size - is size of index in hash-table;
key_size - is size of the key in sizeof(char), can be 0 - it's say that key_size = strlen((char*)key);
hash_function - is pointer to hash function, can be NULL but that say that using standart function HashAllData;
*/

storage_t* init_hash(size_t element_size, size_t table_hash_size, size_t key_size, void* hash_function);

/*
HashAllData recive pointer to some data size of key_size and return hash;
*/

size_t HashAllData(void* Data);


#endif // STORAGE_H
