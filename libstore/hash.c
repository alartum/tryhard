#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct storage_t{
	int (*add)(void* self, const void* key, const void* element);
	void (*drop)(void* self, const void* key);
	void* (*find)(const void* self, const void* key);
	int (*clear)(void* self);
	int (*destroy)(void* self);
	size_t (*size)(void* self);

	void (*next)(void* iterator);
	void (*prev)(void* iterator);
	void* (*end)(void* self);
	void* (*begin)(void* self);
	void* (*getKey)(void* iterator);
	void* (*getElem)(void* iterator);
	int (*iterEquel)(void* iterator1, void* iterator2);

	void* manager;
	size_t _size;
} storage_t;

struct _list{
	struct _list* next;
	struct _list* prev;
	void* element;
	void* key;
} typedef list;

struct hash{
	size_t blsize;
	size_t tbsize;
	size_t (*HashFunc)();
	int (*cmpKey)();
	size_t key_size;
	void** table;
} typedef hash;


struct iterator{
	size_t num;
	list* lst;
	void* self;
} typedef iterator;

size_t size_hash(void* self){
	return ((storage_t*)self)->_size;
}
//from habra
#define BITS_IN_int (sizeof(int) * CHAR_BIT)
#define THREE_QUARTERS  ((int) ((BITS_IN_int * 3) / 4))
#define ONE_EIGHTH ((int) (BITS_IN_int / 8))
#define HIGH_BITS  (~((size_t)(~0) >> ONE_EIGHTH))

size_t HashAllData(const char* element){
	if(element == NULL)
		return -1;
	size_t hash_value, i;
	for(hash_value = 0; *element; ++element){
		hash_value = ( hash_value << ONE_EIGHTH ) + *element;
		if (( i = hash_value & HIGH_BITS ) != 0 )
			hash_value = (hash_value ^ (i >> THREE_QUARTERS)) & ~HIGH_BITS;
	}
	return hash_value;
}

int add_hash  (void* self, const void* key, const void* element){
	if(self == NULL || key == NULL || element == NULL)
		return 1;

	hash* var = ((storage_t*)self)->manager;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key) % var->tbsize;

	size_t key_size = var->key_size;

	if(!key_size){
		while(((char*)key)[key_size++]);
		key_size--;
	}

	if(var->table[tg] == NULL){
		var->table[tg] = (void*)malloc(sizeof(list));
		(((list*)(var->table[tg]))->next) = NULL;
		(((list*)(var->table[tg]))->prev) = NULL;
		(((list*)(var->table[tg]))->element) = (void*)malloc(var->blsize * sizeof(char));
		(((list*)(var->table[tg]))->key) = (void*)malloc(key_size * sizeof(char));
		strncpy((char*)(((list*)(var->table[tg]))->element), (const char*)element, var->blsize);
		strncpy((char*)(((list*)(var->table[tg]))->key), (const char*)key, key_size);
		((storage_t*)self)->_size++;
		return 0;
	}

	list* prev = (list*)(var->table[tg]);
	if(!(((hash*)(((storage_t*)self)->manager))->HashFunc)(key, prev->key)){
		return 1;
	}


	list* tmp = prev->next;

	while(tmp != NULL){
		if(!(((hash*)(((storage_t*)self)->manager))->cmpKey)(key, tmp->key)){
			return 1;
		}
		prev = tmp;
		tmp = tmp->next;
	}

	tmp = (list*)malloc(sizeof(list));
	tmp->prev = prev;
	tmp->prev->next = tmp;
	tmp->next = NULL;
	tmp->element = (void*)malloc(var->blsize * sizeof(char));
	tmp->key = (void*)malloc(key_size * sizeof(char));
	strncpy((char*)(tmp->element), (const char*)element, var->blsize);
	strncpy((char*)(tmp->key), (const char*)key, key_size);


	((storage_t*)self)->_size++;
	return 0;
}

void drop_hash(void* self, const void* key){
	hash* var = ((storage_t*)self)->manager;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key) % var->tbsize;
	size_t key_size = var->key_size;

	if(!key_size){
		while(((char*)key)[key_size++]);
		key_size--;
	}

	list* lst = (list*)(var->table[tg]);

	list* prev = NULL;

	while(lst != NULL){
		if(!(((hash*)(((storage_t*)self)->manager))->cmpKey)(key, lst->key)){
			if(prev != NULL){
				prev->next = lst->next;
			}else{
				var->table[tg] = lst->next;
			}
			if(lst->next != NULL){
				lst->next->prev = prev;
			}
			if(lst->element != NULL){
				free(lst->element);
				lst->element = NULL;
			}
			if(lst->key != NULL){
				free(lst->key);
				lst->key = NULL;
			}
			if(lst){
				free(lst);
				lst = NULL;
			}
			((storage_t*)self)->_size--;
			return;
		}
		prev = lst;
		lst = lst->next;
	}
	return;
}


void* find_hash(const void* self, const void* key){
	hash* var = ((storage_t*)self)->manager;
	size_t tg = (((hash*)(((storage_t*)self)->manager))->HashFunc)(key) % var->tbsize;
	size_t key_size = var->key_size;

	if(!key_size){
		while(((char*)key)[key_size++]);
		key_size--;
	}


	list* lst = (list*)var->table[tg];
	while(lst != NULL){
		if(!(((hash*)(((storage_t*)self)->manager))->cmpKey)(key, lst->key)){
			return lst->element;
		}
		lst = lst->next;
	}
	return NULL;
}

int clear_hash(void* self){
	hash* var = ((hash*)(((storage_t*)self)->manager));
	size_t i = 0;
	size_t size = var->tbsize;
	list* tmp = NULL;
	for(i = 0; i < size; i++){
		tmp = (list*)(var->table[i]);
		var->table[i] = NULL;
		while(tmp != NULL){
			if(tmp->element != NULL){
				free(tmp->element);
			}
			tmp->element = NULL;

			if(tmp->key != NULL){
				free(tmp->key);
			}
			tmp->key = NULL;

			if(tmp->prev != NULL){
				free(tmp->prev);
			}
			tmp->prev = NULL;

			if(tmp->next != NULL){
				tmp = tmp->next;
				if(tmp->prev != NULL){
					free(tmp->prev);
				}
				tmp->prev = NULL;
			}else{
				free(tmp);
				tmp = NULL;
			}
		}
	}
	((storage_t*)self)->_size = 0;
	return 0;
}

int destroy_hash(void* self){
	clear_hash(self);
	if(((hash*)(((storage_t*)self)->manager))->table)
		free(((hash*)(((storage_t*)self)->manager))->table);
	if(((storage_t*)self)->manager)
		free(((hash*)((storage_t*)self)->manager));
	if((storage_t*)self)
		free((storage_t*)self);
	return 0;
}

void next_hash(void* arg){
	iterator* temp = (iterator*)arg;
	size_t i = 0;
	storage_t* self = (storage_t*)(temp->self);
	size_t tbsize = ((hash*)(self->manager))->tbsize;

	if(temp->lst->next){
		temp->lst = temp->lst->next;
		return;
	}
	for(i = temp->num + 1; ; i = (i + 1) % tbsize){
		if(((list*)(((hash*)(((storage_t*)self)->manager))->table[i])) != NULL){
			temp->num = i;
			temp->lst = ((list*)(((hash*)(((storage_t*)self)->manager))->table[i]));
			temp->self = (void*)self;
			return;
		}
	}

	return;
}

void prev_hash(void* arg){
	iterator* temp = (iterator*)arg;
	size_t i = 0;
	storage_t* self = (storage_t*)(temp->self);
	size_t tbsize = ((hash*)(self->manager))->tbsize;
	list* tmp = NULL;

	if(temp->lst->prev){
		temp->lst = temp->lst->prev;
		return;
	}

	for(i = temp->num - 1; ; (i > 0)?(i--):(i = tbsize - 1)){
		tmp = ((list*)(((hash*)(((storage_t*)self)->manager))->table[i]));
		if(tmp != NULL){
			while(tmp->next != NULL){
				tmp = tmp->next;
			}
			temp->num = i;
			temp->lst = tmp;
			temp->self = (void*)self;
			return;
		}
	}


	return;
}

void* begin_hash(void* self){
	size_t i = 0;
	size_t size = (((hash*)(((storage_t*)self)->manager))->tbsize);
	for(i = 0; i < size; i++){
		if(((list*)(((hash*)(((storage_t*)self)->manager))->table[i])) != NULL){
			iterator* temp = malloc(sizeof(iterator));
			temp->num = i;
			temp->lst = ((list*)(((hash*)(((storage_t*)self)->manager))->table[i]));
			temp->self = (void*)self;
			return temp;
		}
	}
	return NULL;
}

void* end_hash(void* self){
	size_t i = 0;
	size_t size = (((hash*)(((storage_t*)self)->manager))->tbsize);
	list* tmp = NULL;
	for(i = size - 1; i >= 0; i--){
		tmp = ((list*)(((hash*)(((storage_t*)self)->manager))->table[i]));
		if(tmp != NULL){
			while(tmp->next != NULL){
				tmp = tmp->next;
			}
			iterator* temp = malloc(sizeof(iterator));
			temp->num = i;
			temp->lst = tmp;
			temp->self = (void*)self;
			return temp;
		}
	}
	return NULL;
}

void* at_hash_elem(void* arg){
	iterator* temp = (iterator*)arg;
	return temp->lst->element;
}

void* at_hash_key(void* arg){
	iterator* temp = (iterator*)arg;
	return temp->lst->key;
}

int equel_iterator_hash(void* arg1, void* arg2){
	iterator* tmp1 = (iterator*)arg1;
	iterator* tmp2 = (iterator*)arg2;
	if(tmp1->num == tmp2->num && tmp1->lst == tmp2->lst && tmp1->self == tmp2->self)
		return 1;
	return 0;
}

storage_t* init_hash(size_t blsize, size_t hash_table_size, size_t key_size, void* hashFunc, void* cmpKey){
	storage_t* tmp = (storage_t*)malloc(sizeof(storage_t));
	tmp->add = add_hash;
	tmp->drop = drop_hash;
	tmp->find = find_hash;
	tmp->clear = clear_hash;
	tmp->destroy = destroy_hash;
	tmp->size = size_hash;
	tmp->_size = 0;

	tmp->next = next_hash;
	tmp->prev = prev_hash;
	tmp->end = end_hash;
	tmp->begin = begin_hash;
	tmp->getKey = at_hash_key;
	tmp->getElem = at_hash_elem;
	tmp->iterEquel = equel_iterator_hash;

	tmp->manager = (void*)malloc(sizeof(hash));
	((hash*)(tmp->manager))->tbsize = hash_table_size;
	((hash*)(tmp->manager))->blsize = blsize;
	((hash*)(tmp->manager))->key_size = key_size;
	((hash*)(tmp->manager))->table = (void**)malloc(hash_table_size * sizeof(void*));
	if(hashFunc != NULL){
		((hash*)(tmp->manager))->HashFunc = hashFunc;
	}else{
		((hash*)(tmp->manager))->HashFunc = HashAllData;
	}
	if(cmpKey != NULL){
		((hash*)(tmp->manager))->cmpKey = cmpKey;
	}else{
		((hash*)(tmp->manager))->cmpKey = strcmp;
	}
	size_t i;
	for(i = 0; i < hash_table_size; i++){
		(((hash*)(tmp->manager))->table[i]) = NULL;
	}
	return tmp;
}
