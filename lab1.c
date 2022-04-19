#include<stdio.h>
#include<pthread.h>

#define NTHREADS 2
#define TAMVETOR 10000
int vetor[TAMVETOR];
int vetorteste[TAMVETOR];
int cont;

//funcao que a thread ira executar
void * tarefa (void * arg) {
   int ident = * (int *) arg;

    for(int i=ident-1;i<TAMVETOR;i+=2){
      vetor[i] = vetor[i] * vetor[i];
    }

   
   pthread_exit(NULL);
}

//funcao principal
int main(void) {
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS]; //identificador local da thread
    for(int i=0;i<TAMVETOR;i++){
      vetor[i]=i+2;
    }

    //vetor para comparação 
    for(int i=0; i<TAMVETOR; i++) {
        vetorteste[i]=vetor[i]*vetor[i];
    }
    //cria as threads 
    for(int i=1; i<=NTHREADS; i++) {
       ident[i-1] = i;
       if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1])) 
          printf("ERRO -- pthread_create\n");
    }
    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }


    //comparando os vetores
    for(int i=0; i<TAMVETOR; i++) {
        if (vetor[i]==vetorteste[i]){
            cont++;
        }else{
            printf("Erro - diferente \n");
        }
    }

    if(cont==TAMVETOR){
        printf("FUNCIONOU \n");
    }else{
        printf(" NÃO FUNCIONOU \n");
    }
    return 0;
}
