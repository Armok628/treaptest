#include "treap.h"
const unsigned long fnv_prime=0x100000001b3;
const unsigned long fnv_offset=0xcbf29ce484222325;
unsigned long fnv_1a(char *str)
{
	unsigned long key=fnv_offset;
	for (char c=*str++;c;c=*str++) {
		key^=c;
		key*=fnv_prime;
	}
	return key;
}
node_t *new_node(node_t *parent,char *sym,void *val)
{
	node_t *n=calloc(1,sizeof(node_t));
	n->parent=parent;
	n->key=fnv_1a(sym);
	n->val=val;
	n->priority=rand();
	return n;
}
tree_t *new_tree(dtor_t destructor)
{
	tree_t *t=calloc(1,sizeof(tree_t));
	t->destructor=destructor;
	return t;
}
static inline int dir_of(node_t *parent,node_t *child)
{
	return child==parent->child[1];
}
static inline int ulcmp(unsigned long a,unsigned long b)
{
	return a==b?0:a>b?1:-1;
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
	unsigned long key=fnv_1a(sym);
	if (!tree->root) {
		tree->root=new_node(NULL,sym,val);
		return;
	}
	node_t *n=tree->root;
	for (;;) {
		int cmp=ulcmp(key,n->key);
		if (!cmp) {
			if (tree->destructor)
				tree->destructor(n->val);
			n->val=val;
			return;
		} else if (!n->child[cmp>0]) {
			n->child[cmp>0]=new_node(n,sym,val);
			n=n->child[cmp>0];
			break;
		} else
			n=n->child[cmp>0];
	}
	// n now points to the inserted node
	while (n->parent&&n->priority>n->parent->priority)
		rotate(n->parent,!dir_of(n->parent,n));
	if (!n->parent)
		tree->root=n;
}
node_t *node_lookup(tree_t *tree,char *sym)
{
	unsigned long key=fnv_1a(sym);
	if (!tree->root)
		return NULL;
	node_t *n=tree->root;
	while (n) {
		int cmp=ulcmp(key,n->key);
		if (!cmp)
			return n;
		n=n->child[cmp>0];
	}
	return NULL;
}
void *lookup(tree_t *tree,char *sym)
{
	node_t *n=node_lookup(tree,sym);
	return n?n->val:NULL;
}
void rotate_down(node_t *node)
{ // Assumes node has children
	int dir;
	if (node->child[0]&&node->child[1]) {
		dir=node->child[0]->priority>node->child[1]->priority;
	} else
		dir=!!node->child[0]; // dir rot moves !dir child up
	rotate(node,dir);
}
void expunge(tree_t *tree,char *sym)
{
	node_t *n=node_lookup(tree,sym);
	if (!n)
		return;
	n->priority=-1;
	if (!n->parent) {
		if (n->child[0]||n->child[1])
			rotate_down(n);
		tree->root=n->parent;
	}
	while (n->child[0]||n->child[1]) {
		rotate_down(n);
	}
	if (n->parent)
		n->parent->child[dir_of(n->parent,n)]=NULL;
	else
		tree->root=NULL;
	if (tree->destructor)
		tree->destructor(n->val);
	free(n);
}
void free_subtree(dtor_t d,node_t *node)
{
	if (!node)
		return;
	free_subtree(d,node->child[0]);
	if (d)
		d(node->val);
	free_subtree(d,node->child[1]);
	free(node);
}
void free_tree(tree_t *tree)
{
	free_subtree(tree->destructor,tree->root);
	free(tree);
}
void subtree_do(node_t *node,void (*callback)(node_t *))
{
	if (!node)
		return;
	subtree_do(node->child[0],callback);
	callback(node);
	subtree_do(node->child[1],callback);
}
void tree_do(tree_t *tree,void (*callback)(node_t *))
{
	subtree_do(tree->root,callback);
}
// Debugging stuff
void subtree_structure(node_t *node,int depth)
{
	/**/if (!node) return;
	for (int i=0;i<depth;i++)
		putchar('|');
	if (!node)  {
		puts("NULL");
		return;
	}
	printf("%lu (%d) => %p\n",node->key,node->priority,node->val);
	subtree_structure(node->child[0],depth+1);
	subtree_structure(node->child[1],depth+1);
}

