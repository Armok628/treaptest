#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/treap.h"
#include "src/timer.h"
#include "src/randword.h"
#define WORD_SIZE (10+rand()%11)
typedef enum {false,true} bool;
typedef struct test_s {
	char *str;
	int val;
	struct test_s *cdr;
} test_t;
test_t *new_test()
{
	test_t *t=malloc(sizeof(test_t));
	t->str=random_word(WORD_SIZE);
	t->val=rand();
	t->cdr=NULL;
	return t;
}
int *intptr_to(int i)
{
	int *p=malloc(sizeof(int));
	*p=i;
	return p;
}
int find_dup_tests(test_t *list,test_t *test)
{
	int i=0;
	for (;list;list=list->cdr)
		i+=!strcmp(list->str,test->str);
	return i-1;
}
void locdump(tree_t *tree)
{ // Print number of buckets in each location
	print_tree(tree,1);
}
int main(int argc,char **argv)
{
	if (argc<3)
		goto NOT_ENOUGH_ARGS;
	// Scan arguments
	int words=0,tests=0;
	bool dump=false,time_only=false,m_expunge=false,no_adds=false;
	unsigned int seed=time(NULL);
	sscanf(argv[1],"%d",&words);
	sscanf(argv[2],"%d",&tests);
	for (int i=1;i<argc;i++) {
		sscanf(argv[i],"words=%d",&words);
		sscanf(argv[i],"tests=%d",&tests);
		sscanf(argv[i],"seed=%u",&seed);
		if (!strcmp(argv[i],"--dump"))
			dump=true;
		if (!strcmp(argv[i],"--time_only"))
			time_only=true;
		if (!strcmp(argv[i],"--expunge"))
			m_expunge=true;
		if (!strcmp(argv[i],"--no_adds"))
			no_adds=true;
	}
	// Initialize tree and test variables
	srand(seed);
	tree_t *tree=new_tree(&free);
	test_t *testlist=new_test(); // Make first test
	test_t *test=testlist;
	// Build expectations and add tree values
	start_timer();
	insert(tree,test->str,intptr_to(test->val));
	if (!time_only&&!no_adds)
		printf("Adding %s as %d\n",test->str,test->val);
	for (int i=1;i<words;i++) {
		test_t *t=new_test();
		if (!time_only&&!no_adds)
			printf("Adding %s as %d\n",t->str,t->val);
		insert(tree,t->str,intptr_to(t->val));
		test->cdr=t;
		test=t;
	}
	printf("\nInsertion phase took %f seconds\n",read_timer());
	// Test expectations
	start_timer();
	int successes=0,failures=0,duplicates=0;
	for (int i=0;i<tests;i++) {
		//printf("Test iteration: %d\n",i);
		for (test=testlist;test;test=test->cdr) {
			//printf("Test: Getting entry for %s\n",t->str);
			int *ptr=(int *)lookup(tree,test->str);
			if (!ptr) {
				printf("Failure: lookup returned null\n");
				failures++;
			} else if (*ptr==test->val) {
				//printf("Success!\n");
				successes++;
			} else {
				int d=find_dup_tests(testlist,test);
				duplicates+=!!d;
				if (!d) {
					printf("Failure: Wrong value for %s\n",test->str);
					failures++;
				}
			}
		}
	}
	// Print report
	if (!time_only) {
		printf("\nSuccesses: %d\nDuplicates: %d\nFailures: %d\n",successes,duplicates,failures);
		double dur=read_timer();
		printf("\nTotal value retrieval time: %f seconds\n",dur);
		printf("\nAverage: %f ms per retrieval\n\n",dur/(words*tests/1000.0));
	} else
		printf("%lf",read_timer());
	if (dump)
		locdump(tree);
	// Manually expunge values if requested
	if (m_expunge) {
		start_timer();
		for (test=testlist;test;test=test->cdr)
			expunge(tree,test->str);
		if (!time_only)
			printf("Total data expunge time: %lf seconds\n\n",read_timer());
		if (dump)
			locdump(tree);
	}
	if (!time_only)
		printf("Seed: %u\n\n",seed);
	// Clean up
	for (test=testlist;test;) { // Free test memory
		test_t *t=test;
		free(t->str);
		test=t->cdr;
		free(t);
	}
	free_tree(tree);
	return 0;
NOT_ENOUGH_ARGS:
	tree=new_tree(NULL);
	if (argc<2) {
		char input[100],str[100];
		long num;
		for (;;) {
			printf("Command: ");
			fgets(input,99,stdin);
			if (!strcmp(input,"quit\n")||(*input=='\n'))
				break;
			else if (sscanf(input,"insert %s %ld",str,&num)==2) {
				printf("Inserting %s as %ld\n\n",str,num);
				insert(tree,strdup(str),(void *)num);
			} else if (sscanf(input,"lookup %s",str)==1) {
				long n=(long)lookup(tree,str);
				if (n)
					printf("%ld\n\n",n);
				else
					printf("%s is undefined or zero\n\n",str);
			} else if (sscanf(input,"expunge %s",str)==1) {
				printf("Expunging %s\n\n",str);
				expunge(tree,str);
			} else if (!strcmp(input,"print_tree\n")) {
				print_tree(tree,1);
			} else if (!strcmp(input,"new_tree\n")) {
				printf("Making new tree\n\n");
				free_tree(tree);
				tree=new_tree(NULL);
			} else
				printf("Unrecognized command or format\n\n");
		}
		free_tree(tree);
	}
	return 0;
}
