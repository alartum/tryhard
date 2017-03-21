#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifndef BINTREE_H
#define BINTREE_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
	void* _manager;
} storage_t;

typedef struct bin_tree_t bin_tree_t;

typedef struct bin_node_t {
  void* key;
  void* data;

  bin_tree_t* tree;

  struct bin_node_t* left;
  struct bin_node_t* right;
  struct bin_node_t* parent;
} bin_node_t;

struct bin_tree_t {
  bin_node_t* root;
  bin_node_t* most_left;
  bin_node_t* most_right;
  size_t size;
  int (*key_cmp)(const void* key1, const void* key2);
};

typedef struct tree_iterator{
    bin_tree_t* bin_tree;
    bin_node_t* bin_node;
} tree_iterator;

void bin_tree_iter_next(void* iterator);
void bin_tree_iter_prev(void* iterator);
void* bin_tree_iter_end(void* self);
void* bin_tree_iter_begin(void* self);
void* bin_tree_iter_getKey(void* iterator);
void* bin_tree_iter_getElem(void* iterator);
int bin_tree_iterEquel(void* iterator1, void* iterator2);
storage_t* init_bin_tree(int (*key_cmp)(const void*, const void*));
bin_node_t* bin_node_create(const void* key, const void* data, bin_tree_t* tree);
void bin_tree_init(bin_tree_t* bin_tree, const void* key, const void* data);
void bin_node_init(bin_node_t* bin_node, const void* key, const void* data, bin_tree_t* tree);
void bin_node_delete_subtree(bin_node_t* bin_node);
int bin_tree_add(void* self, const void* key, const void* data);
void* bin_tree_find(const void* self, const void* key);
int bin_tree_destroy(void* self);
void bin_node_delete(bin_node_t* bin_node);
int bin_tree_clear(void* self);
void bin_tree_drop_(bin_node_t* bin_node, const void* key);
void bin_tree_drop(void* self, const void* key);
void bin_node_set_left (bin_node_t* bin_node, bin_node_t* left);
void bin_node_set_right (bin_node_t* bin_node, bin_node_t* right);
void bin_node_dump (const bin_node_t* bin_node);
void bin_node_to_dot (const bin_node_t* bin_node, FILE* dot_file);
void bin_node_show_dot (const bin_node_t* bin_node);

size_t bin_tree_size (void* self)
{
    storage_t* storage = (storage_t*)self;
    bin_tree_t* bin_tree = (bin_tree_t*)(storage->_manager);
    return bin_tree->size;
}

int key_cmp_str(const void* key1, const void* key2)
{
    return strcmp((char*)key1, (char*)key2);
}

int key_cmp_int(const void* key1, const void* key2)
{
    if (*((int*)key1) < *((int*)key2))
        return -1;
    else if (*((int*)key1) > *((int*)key2))
        return 1;
    else
        return 0;
}

storage_t* init_bin_tree(int (*key_cmp)(const void*, const void*)){
	// storage_t version of the rbtree
	storage_t* bin_tree_casted = (storage_t*)malloc(sizeof(*bin_tree_casted));
	bin_tree_casted->add = (&bin_tree_add);
	bin_tree_casted->drop = (&bin_tree_drop);
	bin_tree_casted->find = (&bin_tree_find);
	bin_tree_casted->destroy = (&bin_tree_destroy);
	bin_tree_casted->clear = (&bin_tree_clear);
	bin_tree_casted->size = (&bin_tree_size);
	bin_tree_casted->next = (&bin_tree_iter_next);
	bin_tree_casted->prev = (&bin_tree_iter_prev);
	bin_tree_casted->end = (&bin_tree_iter_end);
	bin_tree_casted->begin = (&bin_tree_iter_begin);
	bin_tree_casted->getKey = (&bin_tree_iter_getKey);
	bin_tree_casted->getElem = (&bin_tree_iter_getElem);
	bin_tree_casted->iterEquel = (&bin_tree_iterEquel);
	bin_tree_casted->_manager = (void*)malloc(sizeof(bin_tree_t));

    // Cast to bin_tree_t type
    bin_tree_t* bin_tree = (bin_tree_t*)(bin_tree_casted->_manager);
    bin_tree->key_cmp = key_cmp;
    bin_tree->size = 0;
    bin_tree->most_left = NULL;
    bin_tree->most_right = NULL;

	return bin_tree_casted;
}

void bin_tree_init(bin_tree_t* bin_tree, const void* key, const void* data)
{
    assert(bin_tree);
    bin_tree->root = bin_node_create(key, data, bin_tree);
    bin_tree->size = 1;
    bin_tree->most_left = bin_tree->root;
    bin_tree->most_right = bin_tree->root;
    assert(bin_tree->root);
}

bin_node_t* bin_node_create(const void* key, const void* data, bin_tree_t* tree)
{
    bin_node_t* new_node = (bin_node_t*)malloc(sizeof(*new_node));
    assert(new_node);
    bin_node_init(new_node, key, data, tree);
    return new_node;
}

void bin_node_init(bin_node_t* bin_node, const void* key, const void* data, bin_tree_t* tree)
{
    bin_node->parent = NULL;
    bin_node->left   = NULL;
    bin_node->right  = NULL;
    bin_node->data   = (void*)data;
    bin_node->key    = (void*)key;
    bin_node->tree   = tree;
   // bin_node_dump(bin_node);
}

void bin_node_delete_subtree(bin_node_t* bin_node)
{
    // If the node is empty
    if (!bin_node)
        return;

    if (bin_node->left){
        bin_node_delete_subtree(bin_node->left);
        free(bin_node->left);
        bin_node->tree->most_left = bin_node;
        bin_node->tree->size --;
        bin_node->left = NULL;
    }

    if (bin_node->right){
        bin_node_delete_subtree(bin_node->right);
        free(bin_node->right);
        bin_node->tree->most_right = bin_node;
        bin_node->tree->size --;
        bin_node->right = NULL;
    }
}

int bin_tree_add(void* self, const void* key, const void* data){
    assert(self);
    assert(key);
    bin_tree_t* bin_tree = (bin_tree_t*)(((storage_t*)self)->_manager);
    if (!(bin_tree->root)){
        bin_tree->root = bin_node_create(key, data, bin_tree);
        bin_tree->size = 1;
        bin_tree->most_left = bin_tree->root;
        bin_tree->most_right = bin_tree->root;

        return 1;
    }
    bin_node_t* curr = bin_tree->root;
    bin_node_t* parent = NULL;
    int is_greater = 0;
    while (curr){
        is_greater = (*(bin_tree->key_cmp))(curr->key, key);
        if (is_greater == 0)
            break;
        parent = curr;
        if (is_greater < 0)
            curr = curr->right;
        else
            curr = curr->left;

    }
    // Element is already there
    if (curr)
        return 0;

    bin_node_t* new_node = bin_node_create(key, data, bin_tree);
    if (is_greater < 0)
        bin_node_set_right(parent, new_node);
    else
        bin_node_set_left(parent, new_node);
    bin_tree->size++;
    int is_min, is_max;
    is_min = (*(bin_tree->key_cmp))(bin_tree->most_left->key, key);
    is_max = (*(bin_tree->key_cmp))(bin_tree->most_right->key, key);
    if (is_min > 0)
        bin_tree->most_left = new_node;
    if (is_max < 0)
        bin_tree->most_right = new_node;
    return 1;
}

void* bin_tree_find(const void* self, const void* key){
    assert(self);
    assert(key);
    bin_tree_t* bin_tree = (bin_tree_t*)(((storage_t*)self)->_manager);
    bin_node_t* curr = bin_tree->root;
    while (curr && 0 != (*(bin_tree->key_cmp))(curr->key, key))
        if (0 > (*(bin_tree->key_cmp))(curr->key, key))
            curr = curr->right;
        else
            curr = curr->left;
    if (curr)
        return (void*)(curr->data);
    else
        return NULL;
}

int bin_tree_destroy(void* self){
    assert(self);
    bin_tree_t* bin_tree = (bin_tree_t*)(((storage_t*)self)->_manager);
    if (!bin_tree)
        return 1;
    bin_tree_clear(self);
    if (bin_tree->root){
        free(bin_tree->root);
        bin_tree->root = NULL;
    }
    return 1;
}

void bin_node_delete(bin_node_t* bin_node){
    assert(bin_node);
    bin_node_delete_subtree(bin_node);
    if (bin_node == bin_node->tree->most_left)
        bin_node->tree->most_left = bin_node->parent;
    if (bin_node == bin_node->tree->most_right)
        bin_node->tree->most_right = bin_node->parent;
    if (bin_node->parent){
        if (bin_node->parent->left == bin_node)
            bin_node->parent->left = NULL;
        if (bin_node->parent->right == bin_node)
            bin_node->parent->right = NULL;
    }
    free(bin_node);
}

int bin_tree_clear(void* self){
    assert(self);
    bin_tree_t* bin_tree = (bin_tree_t*)(((storage_t*)self)->_manager);
    bin_node_delete_subtree(bin_tree->root);
    if (bin_tree->root){
        free(bin_tree->root);
        bin_tree->root = NULL;
        bin_tree->size--;
    }

    return 0;
}

void bin_tree_drop_(bin_node_t* bin_node, const void* key)
{
    if (!bin_node)
        return;
    int is_greater = (*(bin_node->tree->key_cmp))(bin_node->key, key);
    if (is_greater < 0)
        bin_tree_drop_(bin_node->right, key);
    else if (is_greater > 0)
        bin_tree_drop_(bin_node->left, key);
    else{
        bin_node->tree->size --;
        if (bin_node->right && bin_node->left){
            if (!(bin_node->right->left)){
                bin_node->data  = bin_node->right->data;
                bin_node->key   = bin_node->right->key;
                if (bin_node->right->right)
                    bin_node->right->right->parent = bin_node;
                bin_node_t* right = bin_node->right;
                bin_node->right = bin_node->right->right;
                free(right);
            }
            else{
                bin_node_t* most_left = bin_node->right->left;
                for (; most_left->left; most_left = most_left->left);
                bin_node->data = most_left->data;
                bin_node->key = most_left->key;
                most_left->parent->left = most_left->right;
                if (most_left->right) most_left->right->parent = most_left->parent;
                free(most_left);
            }
        }
        else{
            bin_node_t* child = NULL;
            if (bin_node->right){
                child = bin_node->right;
                if (bin_node == bin_node->tree->most_left){
                    bin_node_t* min = bin_node->right;
                    for (; min->left; min = min->left);
                    bin_node->tree->most_left = min;
                }
            }
            if (bin_node->left){
                if (bin_node == bin_node->tree->most_right){
                    bin_node_t* max = bin_node->left;
                    for (; max->right; max = max->right);
                    bin_node->tree->most_right = max;
                }
                child = bin_node->left;
            }
            if (child){
            // Definitely has a parent
                bin_node->data = child->data;
                bin_node->key = child->key;
                bin_node->right = child->right;
                bin_node->left = child->left;
                free(child);
            }
            else{
                if (bin_node == bin_node->tree->most_right)
                    bin_node->tree->most_right = bin_node->parent;
                if (bin_node == bin_node->tree->most_left)
                    bin_node->tree->most_left = bin_node->parent;
                bin_node_delete(bin_node);
            }
        }
    }
}

void bin_tree_drop(void* self, const void* key){
    assert(self);
    assert(key);

    bin_tree_t* bin_tree = (bin_tree_t*)(((storage_t*)self)->_manager);
    assert(bin_tree);
    int is_in_root = (*(bin_tree->key_cmp))(bin_tree->root->key, key);
    if (bin_tree->root && is_in_root == 0
        && !(bin_tree->root->left) && !(bin_tree->root->right)){
            free(bin_tree->root);
            bin_tree->root = NULL;
            bin_tree->most_right = NULL;
            bin_tree->most_left = NULL;
            bin_tree->size--;
    }
    else
        bin_tree_drop_(bin_tree->root, key);
}

void bin_node_set_left (bin_node_t* bin_node, bin_node_t* left)
{
    assert(bin_node);
    assert(left);
    assert(!(bin_node->left));
    bin_node->left = left;
    left->parent = bin_node;
}

void bin_node_set_right (bin_node_t* bin_node, bin_node_t* right)
{
    assert(bin_node);
    assert(right);
    assert(!(bin_node->right));
    bin_node->right = right;
    right->parent = bin_node;
}

void bin_node_dump (const bin_node_t* bin_node)
{
    assert(bin_node);
    printf (ANSI_COLOR_CYAN "bin_node_t" ANSI_COLOR_RESET" {\n" );
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET "data = %p\n", (void*)bin_node->data);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET "key = %p\n", (void*)bin_node->key);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"left = %p\n", (void*)bin_node->left);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"right = %p\n", (void*)bin_node->right);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"parent = %p\n", (void*)bin_node->parent);
    printf(ANSI_COLOR_GREEN  "  ->" ANSI_COLOR_RESET"tree = %p\n", (void*)bin_node->tree);

    printf ("}\n");
}

void bin_node_to_dot (const bin_node_t* bin_node, FILE* dot_file)
{
    assert(dot_file);
    assert(bin_node);

    fprintf (dot_file, "\t%lu ", (long unsigned)bin_node);
    //bin_node_dump(bin_node);
    fprintf (dot_file, "[label = \"%s : %lu\", shape = circle, fillcolor = \"white\", fontcolor = \"black\", style = filled]\n", (char*)bin_node->key, *((size_t*)bin_node->data));
     if (bin_node->left){
        fprintf (dot_file, "\t%lu -> %lu\n", (long unsigned)bin_node, (long unsigned)bin_node->left);
        bin_node_to_dot(bin_node->left, dot_file);
    }
    if (bin_node->right){
        fprintf (dot_file, "\t%lu -> %lu\n", (long unsigned)bin_node, (long unsigned)bin_node->right);
        bin_node_to_dot(bin_node->right, dot_file);
    }
}

void bin_node_show_dot (const bin_node_t* bin_node)
{
    assert(bin_node);
    FILE* dot_file = fopen("dump.dot", "w");
    fprintf (dot_file, "digraph G {\n\tbgcolor = \"#FCFDFE\"\n");
    bin_node_to_dot(bin_node, dot_file);
    fprintf (dot_file, "}\n");
    fclose(dot_file);

    system("xdot dump.dot");
   system("rm dump.dot");
}

void bin_tree_iter_next(void* iterator){
    tree_iterator* iter = (tree_iterator*)iterator;
    if (iter->bin_node == iter->bin_tree->most_right){
        iter->bin_node = NULL;
        iter->bin_tree = NULL;

        return;
    }
    if (iter->bin_node->right){
        iter->bin_node = iter->bin_node->right;
        while (iter->bin_node->left)
            iter->bin_node = iter->bin_node->left;
    }
    else{
        while (1){
            if (iter->bin_node->parent->left == iter->bin_node){
                iter->bin_node = iter->bin_node->parent;
                return;
            }
            iter->bin_node = iter->bin_node->parent;
        }
    }
}

void bin_tree_iter_prev(void* iterator){
    tree_iterator* iter = (tree_iterator*)iterator;
    if (iter->bin_node == iter->bin_tree->most_left){
        iter->bin_node = NULL;
        iter->bin_tree = NULL;

        return;
    }
    if (iter->bin_node->left){
        iter->bin_node = iter->bin_node->left;
        while (iter->bin_node->right)
            iter->bin_node = iter->bin_node->right;
    }
    else{
        while (1){
            if (iter->bin_node->parent->right == iter->bin_node){
                iter->bin_node = iter->bin_node->parent;
                return;
            }
            iter->bin_node = iter->bin_node->parent;
        }
    }
}

void* bin_tree_iter_end(void* self){
    tree_iterator* iter = (tree_iterator*)malloc(sizeof(*iter));
    iter->bin_tree = NULL;
    iter->bin_node = NULL;

    return (void*)iter;
}
void* bin_tree_iter_begin(void* self){
    tree_iterator* iter = (tree_iterator*)malloc(sizeof(*iter));

    storage_t* storage = (storage_t*)self;
    bin_tree_t* bin_tree = (bin_tree_t*)(storage->_manager);
    iter->bin_tree = bin_tree;
    iter->bin_node = bin_tree->most_left;

    return (void*)iter;
}

void* bin_tree_iter_getKey(void* iterator){
    tree_iterator* iter = (tree_iterator*)iterator;

    return iter->bin_node->key;
}

void* bin_tree_iter_getElem(void* iterator){
    tree_iterator* iter = (tree_iterator*)iterator;

    return iter->bin_node->data;
}

int bin_tree_iterEquel(void* iterator1, void* iterator2){
    tree_iterator* iter1 = (tree_iterator*)iterator1;
    tree_iterator* iter2 = (tree_iterator*)iterator2;
    return (iter1->bin_node == iter2->bin_node && iter1->bin_tree == iter2->bin_tree);
}

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

/*
int main(int argc, char* argv[])
{
    storage_t* bin_tree = init_bin_tree(&key_cmp_str);
    char* strings[] = {"One", "Two", "Three", "Four", "Five", "asdsd", "asfsgafdsg", "adfsgwr", "olkjghg", "[poihugjk"};
    int   numbers[] = {4, 8, 6, 2, 1, 3, 9, 7};

    for (int i = 0; i < ARRAY_SIZE(numbers); i++){
        printf ("Add %d\n", numbers[i]);
        bin_tree->add(bin_tree, &numbers[i], NULL);
              bin_node_show_dot(((bin_tree_t*)(bin_tree->_manager))->root);
    }

    for (int i = 0; i < ARRAY_SIZE(numbers) - 1; i++){
        printf ("Drop %d\n", numbers[i]);
        bin_tree->drop(bin_tree, &numbers[i]);
        bin_node_show_dot(((bin_tree_t*)(bin_tree->_manager))->root);
    }

    bin_tree->destroy(bin_tree);
    return 0;
}*/
#endif // BINTREE_H
