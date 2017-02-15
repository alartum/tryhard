#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define BITS_IN_int     ( sizeof(int) * CHAR_BIT )
#define THREE_QUARTERS  ((int) ((BITS_IN_int * 3) / 4))
#define ONE_EIGHTH      ((int) (BITS_IN_int / 8))
#define HIGH_BITS       ( ~((unsigned int)(~0) >> ONE_EIGHTH ))

struct sequence{
	int (*insert)();
	int (*delete)();
	void* (*find)();
	void* who; 
} typedef sequence;

struct list typedef list;

struct list{
	list* next;
	void* data;
	void* key;
} typedef list;

struct hash{
	size_t blsize;
	size_t tbsize;
	list* table;
} typedef hash;

extern unsigned int HashFunc(const char* data, unsigned int* key_size){
	unsigned int hash_value, i;
	*key_size = 0;
	for(hash_value = 0; *data; ++data){
		hash_value = ( hash_value << ONE_EIGHTH ) + *data;
		if (( i = hash_value & HIGH_BITS ) != 0 )
			hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
		hash_value %= 1024;
		*key_size++;
	}
	return hash_value;
}

extern int insert_hash(void* key, void* data, sequence* _var){
	unsigned int key_size;
	unsigned int tg = HashFunc(key, &key_size);
	key_size++;
	hash* var = _var->who;
	list* lst = &(var->table[tg]);

	while(lst->next != NULL){
		if(!strncmp((char*)key, (char*)lst->key, key_size)){
			return 1;		
		}
		lst = lst->next;
	}

	lst->data = malloc(var->blsize * sizeof(char));
	lst->key = malloc(key_size * sizeof(char));
	strncpy(lst->data, (char*)data, var->blsize);
	strncpy(lst->key, (char*)key, key_size);
	lst->next = malloc(sizeof(list));

	return 0;
}

extern int delete_hash(void* key, sequence* _var){
	unsigned int key_size;
	unsigned int tg = HashFunc(key, &key_size);
	key_size++;
	hash* var = _var->who;

	list* lst = &(var->table[tg]);

	list* prev = NULL;

	while(lst->next != NULL){
		if(!strncmp((char*)key, (char*)lst->key, key_size)){
			if(prev != NULL){
				prev->next = lst->next;
				if(lst->key)
					free(lst->key);
				if(lst->data)
					free(lst->data);
				free(lst);
			}else{
				if(lst->key)
					free(lst->key);
				if(lst->data)
					free(lst->data);
			}
			return 1;		
		}
		prev = lst;
		lst = lst->next;
	}

	return 0;
}


extern void* find_hash(void* key, sequence* _var){
	unsigned int key_size;
	unsigned int tg = HashFunc(key, &key_size);
	key_size++;
	hash* var = _var->who;

	list* lst = &(var->table[tg]);

	while(lst->next != NULL){
		if(!strncmp((char*)key, (char*)lst->key, key_size)){
			return lst->data;		
		}
		lst = lst->next;
	}

	return NULL;
}

sequence init_hash(size_t blsize){
	sequence tmp;
	tmp.insert = (&insert_hash);
	tmp.delete = (&delete_hash);
	tmp.find = (&find_hash);
	tmp.who = (void*)malloc(sizeof(hash));
	((hash*)(tmp.who))->blsize = blsize;
	((hash*)(tmp.who))->table = malloc(1024 * sizeof(list*));

	return tmp;
}

