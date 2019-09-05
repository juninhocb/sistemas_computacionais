#include <pthread.h>
#include <stdio.h>

#define TOTAL 1000
unsigned int sum = 0; /* unsigned só permite valores maiores que 1 */ 

/* thread */
/* nota: as duas threads 1 e 2 irão acessar essa mesma função ao mesmo tempo */ 
void *incr(){
	unsigned int i;
	
	for(i=0;i<TOTAL;i++){
		sum += 1;
	}
	
	return NULL;
}

int main(){

	pthread_t thread1;  /* da um nome a thread de thread 1 */
	pthread_t thread2;  /* idem */

		
	if(pthread_create(&thread1, NULL, incr, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}
	
	if(pthread_create(&thread2, NULL, incr, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}


	if(pthread_join(thread1, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	if(pthread_join(thread2, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	printf("SUM: %d\n", sum);

	return 0;
}