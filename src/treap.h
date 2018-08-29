#ifndef TREAP_H
#define TREAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node {
	char *sym;
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
char *lookup(tree_t *,char *);
void expunge(tree_t *,char *);
void print_tree(tree_t *,int);
void free_tree(tree_t *);
#endif
