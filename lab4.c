#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include<stdbool.h>
#include "timer.h"



int nthreads,dim;
double *vetconc;
int *vet;

bool Primo(int x){
    bool conf;
    if(x%2==1)
        conf=true;
    else
        conf=false;
    return conf;
}

pthread_mutex_t mutex; //variavel de lock para exclusao mutua

void *ProcessaPrimos(void * arg) {
    long int id = (long int) arg; //identificador da thread
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   for(int i=0; i<dim; i++) {
        pthread_mutex_lock(&mutex);

        if (Primo(vet[i]))
            vetconc[i] = sqrt(vet[i]);
        else
            vetconc[i] = vet[i];
        pthread_mutex_unlock(&mutex);
    }
}
int main(int argc, char *argv[]) {
   double *vetseq; //vetor sequencial
   double ini, fim,inis,fims; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   int t,*id;

       //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   id = (int*) malloc(sizeof(int)*nthreads);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   vetseq = (double*) malloc(sizeof(double)*dim);
   vetconc = (double*) malloc(sizeof(double)*dim);
   vet = (int*) malloc(sizeof(int)*dim);
    //preenchendo o vetor
    for(int i=0; i<dim; i++) {
        vet[i]=i;
    }
    //função sequencial
    GET_TIME(inis);
    for(int i=0; i<dim; i++) {
        if (Primo(vet[i]))
            vetseq[i] = sqrt(vet[i]);
        else
            vetseq[i] = vet[i];
    }
    GET_TIME(fims);


  GET_TIME(ini);
  pthread_mutex_init(&mutex, NULL);

  for(t=0; t<nthreads; t++) {
    id[t]=t;
    if (pthread_create(&tid[t], NULL, ProcessaPrimos, (void *) &id[t])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }
  //--espera todas as threads terminarem
  for (t=0; t<nthreads; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  } 
  pthread_mutex_destroy(&mutex);

  GET_TIME(fim);
  printf("Tempo = %lf\n", fims-inis);
  printf("Tempo = %lf\n", fim-ini);
/*  printf("Vetor Sequencial\n");
    for(int i=0; i<dim; i++) {
        printf("%lf ", vetseq[i]);
    }
      printf("\n Vetor Concorrente\n");
        for(int i=0; i<dim; i++) {
        printf("%lf ", vetconc[i]);
    }
*/
  pthread_exit(NULL);
    return 0;
}
