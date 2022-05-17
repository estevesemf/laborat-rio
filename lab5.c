/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so pode imprimir "Bye" depois que duas threads A imprimirem  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  3

/* Variaveis globais */
int x = 3;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread A */
void *A (void *t) {

if(x==5){
  pthread_mutex_lock(&x_mutex);
  printf("1: Volte Sempre!\n");
  
}
pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread A */
void *B (void *t) {
  
  printf("2:Fique a vontade\n");

  if (x<5) { 
     printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     x++;
     printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread A */
void *C (void *t) {  

  if (x<5) { 
     printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     x++;
     printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }

  printf("3: Sente-se, por favor\n");

  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread B */
void *D (void *t) {
  
  if (x<5) { 
     printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     x++;
     printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }

  printf("4:Aceita um copo d'água ?\n");

  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

void *E (void *t) {
  printf("5: Seja Bem vindo\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_mutex_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i;
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  //pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[1], NULL, A, NULL);
  pthread_create(&threads[0], NULL, B, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
