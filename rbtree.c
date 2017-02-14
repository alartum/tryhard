#include <stdio.h>
#include <stdlib.h>
#ifndef RBTREE_H
#define RBTREE_H

typedef struct storage_t{
    void* manager; /**< The data structure responsible for storing and operating the data. */

    void (*add)(void* self, const void* element);
    void (*drop)(void* self, const void* element);
    void* (*find)(const void* self, const void* element);
    void (*delete)(void* self);
} storage_t;


extern void RBTree_drop(void* self, const void* element);
extern void RBTree_add(void* self, const void* element);
extern void* RBTree_find(const void* self, const void* element);
extern void RBTree_delete(void* self);

typedef struct rb_node_t {
  void* info;
  char red; // is red
  struct rb_node_t* left;
  struct rb_node_t* right;
  struct rb_node_t* parent;
} rb_node_t;

typedef struct rb_tree_t {
  rb_node_t* root;
} rb_tree_t;

storage_t* RBTree_ctor(){
	storage_t* rb_new = (storage_t*)malloc(sizeof(*rb_new));
	rb_new->add = (&RBTree_add);
	rb_new->drop = (&RBTree_drop);
	rb_new->find = (&RBTree_find);
	rb_new->delete = (&RBTree_delete);
	rb_new->manager = (void*)malloc(sizeof(rb_tree_t));
 //   rb_tree_t* rb_new_cast = (rb_tree_t*)rb_new;

    printf ("Tree created\n");

	return rb_new;
}

extern void RBTree_add(void* self, const void* element){
    printf ("Tree item added\n");
}

extern void* RBTree_find(const void* self, const void* element){
    printf ("Tree item found\n");

    return NULL;
}

extern void RBTree_delete(void* self){
    printf ("Tree deleted\n");
}

extern void RBTree_drop(void* self, const void* element){
    printf ("Tree item dropped\n");
}


#endif // RBTREE_H
