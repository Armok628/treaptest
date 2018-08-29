#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "src/tree.h"
#include "src/randword.h"
int main(int argc,char **argv)
{
	srand(time(NULL));
	tree_t *t=new_tree();
#define INSERT(s) insert(t,s,s)
	for (int i=0;i<10;i++) {
		INSERT(random_word(5));
	}
	print_tree(t,1);
	putchar('\n');
	print_tree(t,0);
}
