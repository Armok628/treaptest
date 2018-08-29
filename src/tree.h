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
#ifndef TREE_H
#define TREE_H
node_t *new_node(node_t *parent,char *sym,void *val)
{
	node_t *n=calloc(1,sizeof(node_t));
	n->parent=parent;
	n->sym=sym;
	n->val=val;
	n->priority=rand();
	return n;
}
tree_t *new_tree(void)
{
	return calloc(1,sizeof(tree_t));
}
static inline int dir_of(node_t *parent,node_t *child)
{
	return parent->child[0]==child?0:1;
}
void rotate(node_t *root,int dir)
{
	node_t *rparent=root->parent;
	node_t *pivot=root->child[!dir];
	node_t *pchild=pivot->child[dir];
	if (rparent)
		rparent->child[dir_of(rparent,root)]=pivot;
	if (root) {
		root->child[!dir]=pchild;
		root->parent=pivot;
	}
	if (pivot) {
		pivot->child[dir]=root;
		pivot->parent=rparent;
	}
	if (pchild)
		pchild->parent=root;
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
			break;
		} else if (!n->child[cmp>0]) {
			n->child[cmp>0]=new_node(n,sym,val);
			break;
		} else
			n=n->child[cmp>0];
	}
	while (n->parent&&n->priority>n->parent->priority)
		rotate(n->parent,!dir_of(n->parent,n));
	if (!n->parent)
		tree->root=n;
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
void subtree_structure(node_t *node,int depth)
{
	for (int i=0;i<depth;i++)
		putchar('|');
	if (!node)  {
		puts("NULL");
		return;
	}
	printf("%s (%d) => %p\n",node->sym,node->priority,node->val);
	subtree_structure(node->child[0],depth+1);
	subtree_structure(node->child[1],depth+1);
}
void subtree_keys(node_t *node)
{
	if (!node)
		return;
	subtree_keys(node->child[0]);
	puts(node->sym);
	subtree_keys(node->child[1]);
}
void print_tree(tree_t *tree,int struc)
{
	if (tree->root) {
		if (struc)
			subtree_structure(tree->root,0);
		else
			subtree_keys(tree->root);
	} else
		puts("NULL");
}
#endif
