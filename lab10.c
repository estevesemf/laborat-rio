#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
int dim, te,tl;
sem_t escrita,mutex;
int leitores = 0 ;
int *v;


void* leitora(void arg){
tArgs *args = (tArgs*) arg;
sem_wait(&mutex);
leitores++;
if(leitores==1)
    sem_wait(&escrita);
sem_post(&mutex);
printf("Leitor n: %d liberado \n",args->id1);
sem_wait(&mutex);
leitores--;
if (leitores==0)
    sem_post(&escrita);
sem_post(&mutex);   
}

void* escritora(void arg){
tArgs *args = (tArgs*) arg;
sem_wait(&escrita);
printf("Escritor n: %d liberado \n",args->id2);
sem_post(&escrita); 
}
//fluxo principal
int main(int argc, char* argv[]) {
   pthread_t *tid1,*tid2; //identificadores das threads no sistema
   tArgs *args ; //identificadores locais das threads e dimensao
   sem_init(&mutex,0,1);
   sem_init(&escrita,0,0);
   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s  <threads leitoras> <threads escritoras> \n", argv[0]);
      return 1;
   }
   tl = atoi(argv[1]);
   te = atoi(argv[2]);
  
  dim = tl + te ;
   //alocacao de memoria para as estruturas de dados
   v = (int *) malloc(sizeof(int) * dim);
   if (v == NULL) {printf("ERRO--malloc\n"); return 2;}


   //alocacao das estruturas
    tid1 = (pthread_t*) malloc(sizeof(pthread_t)*tl); // threads leitoras
   if(tid1==NULL) {puts("ERRO--malloc"); return 2;}    // threads escritoras
    tid2 = (pthread_t*) malloc(sizeof(pthread_t)*tl);
   if(tid1==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*dim); // numero da thrad leitor pra passar nos parametros
   if(args==NULL) {puts("ERRO--malloc"); return 2;}


   //criacao das threads
   for(int i=0; i<tl; i++) {
      (args+i)->id1 = i;
      if(pthread_create(tid1+i, NULL, leitora, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   }
      //criacao das threads
   for(int i=0; i<te; i++) {
      (args+i+te)->id2 = i;
      if(pthread_create(tid2+i, NULL, escritora, (void*) (args+i+te))){
         puts("ERRO--pthread_create"); return 3;
      }
   }

   //espera pelo termino da threads
    for(int i=0; i<tl; i++) {
      pthread_join(*(tid1+i), NULL);
   }
    for(int i=0; i<te; i++) {
      pthread_join(*(tid2+i), NULL);
   }

   
   free(args2);
   free(args1);
   free(tid1);
   free(tid2);
    
 
   //printf 
   printf("\n");

   return 0;
}
