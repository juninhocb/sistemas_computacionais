#include <stdio.h>
#include "linkedlist.h"


#define LISTSZ 10

int main(){
	struct linkedlist_t *l;
	int i, *n;
	int *aux;

	l = linkedlist_create();

	for(i=0; i<LISTSZ; i++){
		n = malloc(sizeof(int));
		*n = i;
		if(i==5){
			aux = n;
		}
		linkedlist_insert_tail(l, n);
	}

	printf("\n%d\n", linkedlist_size(l));

	linkedlist_remove(l, aux);

	printf("\n%d\n", linkedlist_size(l));

	linkedlist_destroy(&l);

	return 0;


}