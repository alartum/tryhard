#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef RBTREE_H
#define RBTREE_H

typedef struct rb_node_t {
  int info;
  char is_red;
  struct rb_node_t* left;
  struct rb_node_t* right;
  struct rb_node_t* parent;
} rb_node_t;

typedef struct rb_tree_t {
  rb_node_t* root;
} rb_tree_t;

storage_t* RBTree_ctor(size_t element_size){
	// storage_t version of the rbtree
	storage_t* rb_tree_casted = (storage_t*)malloc(sizeof(*rb_tree_casted));
	rb_tree_casted->add = (&RBTree_add);
	rb_tree_casted->drop = (&RBTree_drop);
	rb_tree_casted->find = (&RBTree_find);
	rb_tree_casted->delete = (&RBTree_delete);
	rb_tree_casted->manager = (void*)malloc(sizeof(rb_tree_t));

    // Cast to rb_tree_t type
    rb_tree_t* rb_tree = (rb_tree_t*)rb_tree_casted->manager;
    int is_ok = rb_tree_init(rb_tree);
    assert (if_ok);
    printf ("Tree created\n");
	if (is_ok)
        return rb_tree;
    else
        return NULL;
}

int rb_tree_init(rb_tree_t* rb_tree)
{
    assert(rb_tree);
    rb_tree->root = rb_node_create(0);
    assert(rb_tree->root);
    if (rb_tree->root)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

rb_node_t* rb_node_create(const int info)
{
    rb_node_t* new_node = (rb_node_t*)malloc(sizeof(*new_node));
    assert(new_node);
    rb_node_init(new_node, info);
    return new_node;
}

void rb_node_init(rb_node_t* rb_node, const int info)
{
    rb_node->is_red = 0;
    rb_node->parent = NULL;
    rb_node->left   = NULL;
    rb_node->right  = NULL;
    rb_node->info   = info;
}

void rb_node_delete_subtree(rb_node_t* node)
{
    assert(node);
    rb_node_delete(node->left);
    free(node->left);
    node->left  = NULL;

    rb_node_delete(node->right);
    free(node->right);
    node->right = NULL;
}

// Dummy function at the moment
int rb_node_OK(rb_node_t* node)
{
    assert(node);
    return 1;
}

int RBTree_add(void* self, const void* element){
    printf ("Tree item added\n");

    return 0;
}

void* RBTree_find(const void* self, const void* element){
    printf ("Tree item found\n");

    return NULL;
}

int RBTree_delete(void* self){
    printf ("Tree deleted\n");

    return 0;
}

int RBTree_clear(void* self){
    printf ("Tree cleared\n");

    return 0;
}

void RBTree_drop(void* self, const void* element){
    printf ("Tree item dropped\n");
}


#endif // RBTREE_H
