typedef struct node {
	char *sym;
	void *val;
	struct node *parent;
	struct node *child[2];
} node_t;
typedef struct tree {
	node_t *root;
} tree_t;
tree_t *new_tree(void);
void insert(tree_t *,char *,void *);
char *lookup(tree_t *,char *);
void expunge(tree_t *,char *);
void print_tree(tree_t *);
#ifndef TREE_H
#define TREE_H
node_t *new_node(node_t *parent,char *sym,void *val)
{
	node_t *n=calloc(1,sizeof(node_t));
	n->parent=parent;
	n->sym=sym;
	n->val=val;
	return n;
}
tree_t *new_tree(void)
{
	return calloc(1,sizeof(tree_t));
}
void insert(tree_t *tree,char *sym,void *val)
{
	if (!tree->root) {
		tree->root=new_node(NULL,sym,val);
		return;
	}
	node_t *n=tree->root;
	for (;;) {
		int cmp=strcmp(sym,n->sym);
		if (!cmp) {
			n->val=val;
			return;
		} else if (!n->child[cmp>0]) {
			n->child[cmp>0]=new_node(n,sym,val);
			return;
		} else
			n=n->child[cmp>0];
	}
}
node_t *node_lookup(tree_t *tree,char *sym)
{
	if (!tree->root)
		return NULL;
	node_t *n=tree->root;
	while (n) {
		int cmp=strcmp(sym,n->sym);
		if (!cmp)
			return n;
		n=n->child[cmp>0];
	}
	return NULL;
}
char *lookup(tree_t *tree,char *sym)
{
	return node_lookup(tree,sym)->val;
}
void remove_node(node_t *child)
{
	node_t *parent=child->parent;
	int dir=child==parent->child[0]?0:1;
	if (!child)
		return;
	if (child->child[0]&&child->child[1]) {
		node_t *leftmost=child->child[0];
		for (;leftmost->child[0];leftmost=leftmost->child[0]);
		remove_node(leftmost);
		parent->child[dir]=leftmost;
		leftmost->parent=parent;
	} else if (child->child[0]||child->child[1]) {
		node_t *grandchild=child->child[child->child[0]?0:1];
		parent->child[dir]=grandchild;
		grandchild->parent=parent;
	} else {
		parent->child[dir]=NULL;
	}
	free(child);
}
void expunge(tree_t *tree,char *sym)
{
	remove_node(node_lookup(tree,sym));
}
void print_node(node_t *node,int depth)
{
	if (!node) 
		return;
	print_node(node->child[0],depth+1);
	printf("%s => %p\n",node->sym,node->val);
	print_node(node->child[1],depth+1);
}
void print_tree(tree_t *tree)
{
	if (tree->root)
		print_node(tree->root,0);
	else
		puts("NULL");
}
#endif
