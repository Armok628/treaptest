#ifndef TREAP_H
#define TREAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node {
	unsigned long key;
	void *val;
	struct node *parent; // TODO: Remove
	struct node *child[2];
	int priority;
} node_t;
typedef void (*dtor_t)(void *);
typedef struct tree {
	dtor_t destructor;
	node_t *root;
} tree_t;

tree_t *new_tree(dtor_t);
void insert(tree_t *,char *,void *);
void *lookup(tree_t *,char *);
void expunge(tree_t *,char *);
void free_tree(tree_t *);
void tree_do(tree_t *,void (*)(node_t *));
void print_structure(tree_t *);
void print_tree(tree_t *);
/**/void subtree_structure(node_t *,int);
#endif
