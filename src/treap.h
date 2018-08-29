#ifndef TREAP_H
#define TREAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node {
	char *sym;
	void *val;
	struct node *parent;
	struct node *child[2];
	int priority;
} node_t;
typedef struct tree {
	node_t *root;
} tree_t;

tree_t *new_tree(void);
void insert(tree_t *,char *,void *);
char *lookup(tree_t *,char *);
void print_tree(tree_t *,int);
#endif

