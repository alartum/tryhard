#include <stdlib.h>

#ifndef STORAGE_H
#define STORAGE_H

#ifdef __cplusplus
extern "C"{
#endif
/**
@brief storage with search function
An interface for different approaches of data storage (hash table, black-red tree)
*/
#pragma pack(push, 8)
typedef struct storage_t{
//Main metod's
	int (*add)(void* self, const void* key, const void* element);
	void (*drop)(void* self, const void* key);
	void* (*find)(const void* self, const void* key);
	int (*clear)(void* self);
	int (*destroy)(void* self);
	size_t (*size)(void* self);
//Metod's for iterator
	void (*next)(void* iterator);
	void (*prev)(void* iterator);
	void* (*end)(void* self);
	void* (*begin)(void* self);
	void* (*getKey)(void* iterator);
	void* (*getElem)(void* iterator);
	int (*iterEquel)(void* iterator1, void* iterator2);

} storage_t;
#pragma(pop)


storage_t* RBTree_ctor(size_t element_size);

/*
Function for init storage_t working on hash table.
element_size - is size of the save element in sizeof(char);
table_hash_size - is size of index in hash-table;
key_size - is size of the key in sizeof(char), can be 0 - it's say that key_size = strlen((char*)key);
hash_function - is pointer to hash function, can be NULL but that say that using standart function HashAllData;
cmp_key_func - is pointer to cmp  function when cmp 2 key value and return 0 if equel and another return not 0
*/

storage_t* init_hash(size_t element_size, size_t table_hash_size, size_t key_size, void* hash_function, void* cmp_key_func);

/*
HashAllData recive pointer to some data size of key_size and return hash;
*/

size_t HashAllData(void* Data);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_H
