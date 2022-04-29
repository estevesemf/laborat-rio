#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //matriz 1 de entrada
float *mat2; //matriz 2 de entrada
float *sconc; //matriz que sai da implementação concorrente
float *sseq; // implementação sequencial
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao das threads que executará a multiplicação das matrizes
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   
   for(int i=args->id; i<args->dim; i+=nthreads){
      for(int j=0; j<args->dim; j++) {
         for(int p=0; p<args->dim;p++)
            sconc[i*(args->dim)+j] += mat1[i*(args->dim) + p] * mat2[p*(args->dim) +j];
      }   
   }
   pthread_exit(NULL);
}


int checa(float *a, float *b,int n){
   for(int i = 0; i<n;i++){
      for(int j = 0; j<n;j++){
         if(sconc[i*n+j] !=  sseq[i*n+j]){
            return 2;
         }   
      }
   }
   return 1;
}
//fluxo principal
int main(int argc, char* argv[]) {
   int dim; //dimensao da matriz de entrada
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta,tconc,tseq;
   

   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat2 = (float *) malloc(sizeof(float) * dim *dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   sconc = (float *) malloc(sizeof(float) * dim * dim);
   if (sconc == NULL) {printf("ERRO--malloc\n"); return 2;}
   sseq = (float *) malloc(sizeof(float) * dim * dim);
   if(sseq == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat1[i*dim+j] = rand();    //equivalente mat1[i][j]
         mat2[i*dim+j] = rand();    //equivalente mat2[i][j]    
         sconc[i*dim+j] = 0;
         sseq[i*dim+j] = 0;
      }
   }

   GET_TIME(inicio);
   //multiplicacao sequencial da matriz 1 pela matriz 2
   for(int i=0;i<dim;i++){
      for(int j=0;j<dim;j++){
         for(int p=0;p<dim;p++) sseq[i*dim+j] += mat1[i*dim+p] * mat2[p*dim+j];
            
      }
   }
  
   GET_TIME(fim);
   delta = fim - inicio;
   tseq = delta;
   printf("Tempo multiplicacao sequencial: %lf\n", delta);

   //multiplicacao concorrente da matriz 1 pela matriz 2
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   tconc = delta;
   printf("Tempo multiplicacao concorrente: %lf\n", delta);

   //função que checa as saidas
   if(checa(mat1,mat2,dim) == 1 ){
         puts("Matrizes iguais");
   }
   if (checa(mat1,mat2,dim) ==2 ){
         puts("ERRO-- Matrizes diferentes");
   }

   //liberacao da memoria

   free(mat1);
   free(mat2);
   free(sconc);
   free(sseq);
   free(args);
   free(tid);

   printf("O ganho de desempenho foi de: %lf\n", tseq/tconc);

   return 0;
}
