#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef RBTREE_H
#define RBTREE_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/// More comfortable dump
#define RBTree_node_dump(This) RBTree_node_dump_(This, #This)

typedef struct storage_t storage_t;

#pragma pack(push, 8)
struct storage_t{
//Main metod's
	int (*add)(storage_t* self, const void* key, const void* element);
	void (*drop)(storage_t* self, const void* key);
	void* (*find)(const storage_t* self, const void* key);
	int (*clear)(storage_t* self);
	int (*destroy)(storage_t* self);
	size_t (*size)(storage_t* self);
//Metod's for iterator
	void (*next)(void* iterator);
	void (*prev)(void* iterator);
	void* (*end)(storage_t* self);
	void* (*begin)(storage_t* self);
	void* (*getKey)(void* iterator);
	void* (*getElem)(void* iterator);
	int (*iterEquel)(void* iterator1, void* iterator2);

    void* _manager;
    size_t _size;
};
#pragma(pop)

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

storage_t* rb_tree_ctor(size_t element_size);
void rb_node_init(rb_node_t* rb_node, const int info);
void rb_tree_init(rb_tree_t* rb_tree, const int info);
rb_node_t* rb_node_create(const int info);
void rb_node_delete_subtree(rb_node_t* node);
int rb_node_OK(rb_node_t* node);
int rb_tree_add(storage_t* self, const void* key, const void* element);
void* rb_tree_find(const storage_t* self, const void* key);
int rb_tree_destroy(storage_t* self);
int rb_tree_clear(storage_t* self);
void rb_tree_drop(storage_t* self, const void* key);
void rb_node_set_left (rb_node_t* node, rb_node_t* left);
void rb_node_set_right (rb_node_t* node, rb_node_t* right);
void rb_node_to_dot (const rb_node_t* node, FILE* dot_file);
void rb_node_show_dot (const rb_node_t* node);
void rb_node_dump (const rb_node_t* rb_node);

storage_t* rb_tree_ctor(size_t element_size){
	// storage_t version of the rbtree
	storage_t* rb_tree_casted = (storage_t*)malloc(sizeof(*rb_tree_casted));
	rb_tree_casted->add = (&rb_tree_add);
	rb_tree_casted->drop = (&rb_tree_drop);
	rb_tree_casted->find = (&rb_tree_find);
	rb_tree_casted->destroy = (&rb_tree_destroy);
	rb_tree_casted->_manager = (void*)malloc(sizeof(rb_tree_t));

    // Cast to rb_tree_t type
    //rb_tree_t* rb_tree = (rb_tree_t*)rb_tree_casted->_manager;
    //printf ("Tree created\n");

	return rb_tree_casted;
}

void rb_tree_init(rb_tree_t* rb_tree, const int info)
{
    assert(rb_tree);
    rb_tree->root = rb_node_create(info);
    assert(rb_tree->root);
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
   // rb_node_dump(rb_node);
}

void rb_node_delete_subtree(rb_node_t* rb_node)
{
    // If the node is empty
    if (!rb_node)
        return;

    rb_node_delete_subtree(rb_node->left);
    free(rb_node->left);
    rb_node->left = NULL;

    rb_node_delete_subtree(rb_node->right);
    free(rb_node->right);
    rb_node->right = NULL;
}

int rb_tree_add(storage_t* self, const void* key, const void* element){
    assert(self);
    assert(key);
//    assert(element);
    rb_tree_t* rb_tree = (rb_tree_t*)(self->_manager);
    int info = *((int*)key);

    if (!(rb_tree->root)){
        rb_tree->root = rb_node_create(info);

        return 1;
    }

    rb_node_t* curr = rb_tree->root;
    rb_node_t* parent = NULL;
    while (curr && curr->info != info){
        parent = curr;
        if (curr->info < info)
            curr = curr->right;
        else
            curr = curr->left;
    }

    // Element is already there
    if (curr)
        return 0;

    rb_node_t* new_node = rb_node_create(info);
    if (parent->info < info)
        rb_node_set_right(parent, new_node);
    else
        rb_node_set_left(parent, new_node);

    return 1;
}

void* rb_tree_find(const storage_t* self, const void* key){
    assert(self);
    assert(key);
    rb_tree_t* rb_tree = (rb_tree_t*)(self->_manager);
    rb_node_t* curr = rb_tree->root;
    int info = *((int*)key);
    while (curr && curr->info != info)
        if (curr->info < info)
            curr = curr->right;
        else
            curr = curr->left;
    return (void*)curr;
}

int rb_tree_destroy(storage_t* self){
    assert(self);
    rb_tree_t* rb_tree = (rb_tree_t*)(self->_manager);
    if (!rb_tree)
        return 1;
    rb_tree_clear(self);
    if (rb_tree->root){
        free(rb_tree->root);
        rb_tree->root = NULL;
    }
    return 1;
}

void rb_node_delete(rb_node_t* rb_node){
    assert(rb_node);
    rb_node_delete_subtree(rb_node);
    if (rb_node->parent){
        if (rb_node->parent->left == rb_node)
            rb_node->parent->left = NULL;
        if (rb_node->parent->right == rb_node)
            rb_node->parent->right = NULL;
    }
    free(rb_node);
}

int rb_tree_clear(storage_t* self){
    assert(self);
    rb_tree_t* rb_tree = (rb_tree_t*)(self->_manager);
    rb_node_delete_subtree(rb_tree->root);
    rb_tree->root->info = 0;

    return 0;
}

void rb_tree_drop_(rb_node_t* rb_node, const void* key)
{
    if (!rb_node)
        return;

    int info = *((int*)key);
    if (info > rb_node->info)
        rb_tree_drop_(rb_node->right, key);
    else if (info < rb_node->info)
        rb_tree_drop_(rb_node->left, key);
    else{
        if (rb_node->right && rb_node->left){
            if (!rb_node->right->left){

                rb_node->info  = rb_node->right->info;
                rb_node->is_red = rb_node->right->is_red;
                if (rb_node->right->right)
                    rb_node->right->right->parent = rb_node;
                rb_node->right = rb_node->right->right;
                free(rb_node->right);
                return;
            }
            else{
                rb_node_t* most_left = rb_node->right->left;
                for (; most_left->left; most_left = most_left->left);
                rb_node->info = most_left->info;
                rb_node->is_red = most_left->is_red;
                rb_node_delete(most_left);
                return;
            }
        }

        rb_node_t* child = NULL;
        if (rb_node->right)
            child = rb_node->right;
        if (rb_node->left)
            child = rb_node->left;
        if (child){
        // Definitely has a parent
            rb_node->info = child->info;
            rb_node->is_red = child->is_red;
            rb_node->right = child->right;
            rb_node->left = child->left;
            free(child);
            return;
        }
        else{
            rb_node_delete(rb_node);
            return;
        }
    }
}

void rb_tree_drop(storage_t* self, const void* key){
    assert(self);
    assert(key);

    rb_tree_t* rb_tree = (rb_tree_t*)(self->_manager);
    assert(rb_tree);
    if (rb_tree->root && rb_tree->root->info == *((int*)key)
                      && !(rb_tree->root->left)
                      && !(rb_tree->root->right)){

        free(rb_tree->root);
        rb_tree->root = NULL;
    }
    else
        rb_tree_drop_(rb_tree->root, key);
}

void rb_node_set_left (rb_node_t* rb_node, rb_node_t* left)
{
    assert(rb_node);
    assert(left);
    assert(!(rb_node->left));
    rb_node->left = left;
    left->parent = rb_node;
}

void rb_node_set_right (rb_node_t* rb_node, rb_node_t* right)
{
    assert(rb_node);
    assert(right);
    assert(!(rb_node->right));
    rb_node->right = right;
    right->parent = rb_node;
}

void rb_node_dump (const rb_node_t* rb_node)
{
    assert(rb_node);
    printf (ANSI_COLOR_CYAN "rb_node_t" ANSI_COLOR_RESET" {\n" );
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET "info = %d\n", rb_node->info);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"is_red = %d\n", (int)rb_node->is_red);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"left = %p\n", (void*)rb_node->left);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"right = %p\n", (void*)rb_node->right);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"parent = %p\n", (void*)rb_node->parent);

    printf ("}\n");
}

void rb_node_to_dot (const rb_node_t* rb_node, FILE* dot_file)
{
    assert(dot_file);
    assert(rb_node);

    fprintf (dot_file, "\t%lu ", (long unsigned)rb_node);
    //rb_node_dump(rb_node);
    if (rb_node->is_red)
        fprintf (dot_file, "[label = \"%d\", shape = circle, fillcolor = \"red\", fontcolor = \"black\", style = filled]\n", rb_node->info);
    else
        fprintf (dot_file, "[label = \"%d\", shape = circle, fillcolor = \"black\", fontcolor = \"red\", style = filled]\n", rb_node->info);
    if (rb_node->left){
        fprintf (dot_file, "\t%lu -> %lu\n", (long unsigned)rb_node, (long unsigned)rb_node->left);
        rb_node_to_dot(rb_node->left, dot_file);
    }
    if (rb_node->right){
        fprintf (dot_file, "\t%lu -> %lu\n", (long unsigned)rb_node, (long unsigned)rb_node->right);
        rb_node_to_dot(rb_node->right, dot_file);
    }
}

void rb_node_show_dot (const rb_node_t* rb_node)
{
    assert(rb_node);
    FILE* dot_file = fopen("dump.dot", "w");
    fprintf (dot_file, "digraph G {\n\tbgcolor = \"#FCFDFE\"\n");
    rb_node_to_dot(rb_node, dot_file);
    fprintf (dot_file, "}\n");
    fclose(dot_file);
    system("xdot dump.dot");
   // system("rm dump.dot");
}

int main(int argc, char* argv[])
{
    storage_t* rb_tree = rb_tree_ctor(0);
    int a[] = {1, 8, 3, 9, 4, 2, 5, 0};

    for (int i = 0; i < 8; i++){
            printf ("Add %d\n", a[i]);
        rb_tree->add(rb_tree, &a[i], NULL);
        rb_node_show_dot(((rb_tree_t*)(rb_tree->_manager))->root);
    }
    for (int i = 0; i < 8; i++){
        printf ("Drop %d\n", a[i]);
        rb_tree->drop(rb_tree, &a[i]);
        rb_node_show_dot(((rb_tree_t*)(rb_tree->_manager))->root);
    }
    rb_tree->destroy(rb_tree);
    return 0;
}
#endif // RBTREE_H
