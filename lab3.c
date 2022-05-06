#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include"timer.h"

int nthreads;
long int dim;
int *vetor;

typedef struct{
   long int menor;
   long int maior;
  } mem ;

//fluxo das threads
void* tarefa (void* arg) {
   long int id = (long int) arg;
   long int tam_bloco = dim/nthreads;
   long int ini = id*tam_bloco;
   long int fim = ini+tam_bloco;
    mem memlocal ;
   memlocal.maior = 0;
   memlocal.menor = 10000000;
    if (id==nthreads-1) fim=dim; //trata sobra
   for(long int i=ini; i<fim; i++){
      if(memlocal.maior < vetor[i]){
          memlocal.maior = vetor[i];
      }
      if(memlocal.menor > vetor[i]){
        memlocal.menor = vetor[i];
      }
   }

   pthread_exit((void*)memlocal);

}

//fluxo principal
int main(int argc, char *argv[]) {
   long int menor_seq,maior_seq ;
   long int maior_conc,menor_conc ;
   double ini, fim; //tomada de tempo
   pthread_t *tid;
    mem retorno;
   long int retorno;
   retorno.maior = 0;
   retorno.menor = 0;
   maior_seq = 0;
   menor_seq = 1000000;
   maior_conc = 0;
   menor_conc =1000000;
   //verifica os parametros
   if(argc<3){
      fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca espaco e preenche o vetor
   vetor = (int*) malloc(sizeof(int)*dim);
   if(vetor==NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   for (int i=0; i<dim; i++)
      vetor[i] = i/1000;

   //solucao sequencial FAZER ******
   GET_TIME(ini);

   for (long int i=0; i<dim; i++){
       if(maior_seq < vetor[i]){
            maior_seq = vetor[i];
      }
      if(menor_seq > vetor[i]){
        menor_seq = vetor[i];
      }
   }
  
   GET_TIME(fim);
   printf("Tempo-Seq: %lf\n", fim-ini);

   //solucao concorrente
   GET_TIME(ini);
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   for(long int i=0; i<nthreads; i++){
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         fprintf(stderr, "ERRO--pthread_create\n");
         return 2;
      }   
   }
   for(long int i=0; i<nthreads; i++){
      if(pthread_join(*(tid+i), (void**)&retorno)) {
         fprintf(stderr, "ERRO--pthread_join\n");
         return 2;
      }
      if(retorno.maior>maior_conc){   
        maior_conc=retorno.maior;
      }
      if(retorno.menor<menor_conc){
        menor_conc=retorno.menor;
      }

   }
   GET_TIME(fim);
   printf("Tempo-Conc: %lf\n", fim-ini);

   //exibe os resultados
   printf("Maior e menor Seq:  %ld %ld\n", maior_seq, menor_seq);
   printf("Maior e menor conc: %ld %ld\n", maior_conc , menor_conc);

   free(tid);
   free(vetor);

   return 0;
}
