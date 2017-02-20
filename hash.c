#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define BITS_IN_int (sizeof(int) * CHAR_BIT)
#define THREE_QUARTERS  ((int) ((BITS_IN_int * 3) / 4))
#define ONE_EIGHTH ((int) (BITS_IN_int / 8))
#define HIGH_BITS  (~((size_t)(~0) >> ONE_EIGHTH))

typedef struct storage_t{
	int (*add)(void* self, const void* key, const void* element);
	void (*drop)(void* self, const void* key);
	void* (*find)(const void* self, const void* key);
	int (*clear)(void* self);
	int (*delete)(void* self);
	void* manager;
	size_t size;
} storage_t;

struct list typedef list;

struct list{
	list* next;
	void* element;
	void* key;
} typedef list;

struct hash{
	size_t blsize;
	size_t tbsize;
	size_t (*HashFunc)();
	list* table;
} typedef hash;

size_t HashAllData(const char* element, size_t* key_size, size_t size){
	size_t hash_value, i;
	for(hash_value = 0; *element; ++element){
		hash_value = ( hash_value << ONE_EIGHTH ) + *element;
		if (( i = hash_value & HIGH_BITS ) != 0 )
			hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
		hash_value %= size;
		*key_size++;
	}
	return hash_value;
}

int add_hash  (void* self, const void* key, const void* element){
	size_t key_size = 1;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key, &key_size, ((storage_t*)self)->size);
	key_size++;
	hash* var = ((storage_t*)self)->manager;
	list* lst = &(var->table[tg]);

	while(lst->next != NULL){
		if(!strncmp((const char*)key, (const char*)lst->key, key_size)){
			return 1;		
		}
		lst = lst->next;
	}

	lst->element = (void*)malloc(var->blsize * sizeof(char));
	lst->key = (void*)malloc(key_size * sizeof(char));
	strncpy((char*)lst->element, (const char*)element, var->blsize);
	strncpy((char*)lst->key, (const char*)key, key_size);
	lst->next = (list*)malloc(sizeof(list));
	lst->next->next = NULL;
	lst->next->element = NULL;
	lst->next->key = NULL;
	return 0;
}

void drop_hash(void* self, const void* key){
	size_t key_size = 1;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key, &key_size, ((storage_t*)self)->size);
	key_size++;
	hash* var = ((storage_t*)self)->manager;

	list* lst = &(var->table[tg]);

	list* prev = NULL;

	while(lst->next != NULL){
		if(!strncmp((const char*)key, (const char*)lst->key, key_size)){
			if(prev != NULL){
				prev->next = lst->next;
				if(lst->key)
					free(lst->key);
				if(lst->element)
					free(lst->element);
				if(lst)
					free(lst);
			}else{
				if(lst->key)
					free(lst->key);
				if(lst->element)
					free(lst->element);
			}
			return;		
		}
		prev = lst;
		lst = lst->next;
	}
	return;
}


void* find_hash(const void* self, const void* key){
	size_t key_size = 1;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key, &key_size, ((storage_t*)self)->size);
	key_size++;
	hash* var = ((storage_t*)self)->manager;

	list* lst = &(var->table[tg]);
	while(lst->next != NULL){
		if(!strncmp((char*)key, (char*)lst->key, key_size)){
			return lst->element;		
		}
		lst = lst->next;
	}
	return NULL;
}

int clear_hash(void* self){
	hash* var = (hash*)((storage_t*)self)->manager;
	size_t i = 0;
	size_t size = ((storage_t*)self)->size;
	list* tmp = NULL;
	list* temp = NULL;
	for(i = 0; i < size; i++){
		tmp = &(var->table[i]);
		if(tmp->element){
			free(tmp->element);
			tmp->element = NULL;
		}
		if(tmp->key){
			free(tmp->key);
			tmp->key = NULL;
		}
		while(tmp->next != NULL){
			temp = tmp->next;
			tmp->next = tmp->next->next;	
			if(temp->element){
				free(temp->element);
				temp->element = NULL;
			}
			if(temp->key){
				free(temp->key);
				temp->key = NULL;
			}
			free(temp);
		}		
	}
	
	return 0;
}

int delete_hash(void* self){
	clear_hash(self);
	((storage_t*)self)->add = NULL;
	((storage_t*)self)->drop = NULL;
	((storage_t*)self)->find = NULL;
	((storage_t*)self)->clear = NULL;
	((storage_t*)self)->delete = NULL;
	if(((hash*)(((storage_t*)self)->manager))->table)
		free(((hash*)(((storage_t*)self)->manager))->table);
	if(((storage_t*)self)->manager)
		free(((hash*)((storage_t*)self)->manager));
	if((storage_t*)self)
		free((storage_t*)self);
	return 0;
}


storage_t* init_hash(size_t blsize, size_t hash_table_size, void* hashFunc){
	storage_t* tmp = (storage_t*)malloc(sizeof(storage_t));
	tmp->add = add_hash;
	tmp->drop = drop_hash;
	tmp->find = find_hash;
	tmp->clear = clear_hash;
	tmp->delete = delete_hash;
	tmp->manager = (void*)malloc(sizeof(hash));
	tmp->size = hash_table_size;
	((hash*)(tmp->manager))->blsize = blsize;
	((hash*)(tmp->manager))->table = (list*)malloc(hash_table_size * sizeof(list));
	((hash*)(tmp->manager))->HashFunc = hashFunc;
	size_t i;	
	for(i = 0; i < hash_table_size; i++){
		(((hash*)(tmp->manager))->table[i]).next = NULL;
		(((hash*)(tmp->manager))->table[i]).element = NULL;
		(((hash*)(tmp->manager))->table[i]).key = NULL;
	}
		
	return tmp;
}

