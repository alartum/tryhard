#include <stdio.h>
#include "storage.h"

int main(int argc, char* argv[])
{
    storage_t* rb_tree = RBTree_ctor();

    int a = 0;
    rb_tree->add(&rb_tree, (void*)&a);

    return 0;
}
