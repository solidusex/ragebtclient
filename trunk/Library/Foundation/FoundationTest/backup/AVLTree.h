#pragma once

#include "Foundation.h"





typedef struct __avl_node_tag avl_node_t;

typedef struct __avl_tree_tag avl_tree_t;


avl_tree_t* avl_tree_create();

void print_tree(avl_tree_t *self);

avl_node_t* avl_insert_equal(avl_tree_t *self, int data);


bool avl_erase_node(avl_tree_t *self, int data);


bool avl_verify(avl_tree_t *self);