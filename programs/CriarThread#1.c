#include <pthread.h>
#include <stdio.h>

/* Programa que a thread chama e executa simultaneamente ao incremento de Y */
void *inc_x(void *x_void_ptr){

	
	int *x_ptr = (int *)x_void_ptr;
	while(++(*x_ptr) < 100);

	printf("x increment finished\n");

	/* the function must return something - NULL will do */
	return NULL;
}

int main(){

	int x = 0, y = 0;

	/* Mostra os valores inicias de X e Y */
	printf("x: %d, y: %d\n", x, y);

	/* Da o nome para thread de inc_x_thread */
	pthread_t inc_x_thread;

	/* Cria a Thread que irá incrementar X */
	/* 1 parametro eh o nome da thread, atributos da thread, nome da função que vai chamar, nome da que pode ou nao usar */
	if(pthread_create(&inc_x_thread, NULL, inc_x, &x)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}

	/* Enquanto y for menor que 100, incrementa y */
	while(y < 100){
	y++;
	}

	printf("y increment finished\n");

	/* Não finaliza o programa após o Main acabar, espera a thread terminar de executar  */
	if(pthread_join(inc_x_thread, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;

	}

	/* Mostra os resultados */
	printf("x: %d, y: %d\n", x, y);

	return 0;

}