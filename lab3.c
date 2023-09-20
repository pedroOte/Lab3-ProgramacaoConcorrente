/*
Pedro Luis Mello Otero
121074528
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int nthreads;
int contador = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void barreira(int id){
    pthread_mutex_lock(&x_mutex);
    contador++;

    if(contador < nthreads){
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    else{
        contador = 0;
        pthread_cond_broadcast(&x_cond);
    }

    pthread_mutex_unlock(&x_mutex);
}

void *tarefa(void *i){
    long int id = (long int) i; //identifica a thread


    printf("Ola da thread %ld\n", id + 1);

    barreira(id);

    printf("Que dia bonito %ld\n", id + 1);

    barreira(id);

    printf("Ate breve da thread %ld\n",id + 1);

    barreira(id);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    long int *idThread;
    pthread_t *tid; //indentificador das threads

    if(argc < 2){
        printf("O numero de threads deve ser passado como argumento da funcao.\n");
        return 1;
    }

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    //aloca espaço para as estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL){
        printf("Erro no malloc\n");
        return 2;
    }
    idThread = (long int*) malloc(sizeof(long int) * nthreads);
    if (idThread == NULL){
        printf("Erro no malloc\n");
        return 2;
    }

    nthreads = atoi(argv[1]);

    //cria as threads
    for(int i = 0; i < nthreads; i++){
        pthread_create(&tid[i], NULL, tarefa, (void*) i);
    }

    //junta as threads
    for(int i = 0; i < nthreads; i++){
        if(pthread_join(tid[i], NULL)){
            printf("Erro no Join\n");
            return 3;
        }
    }

    free(tid);
    free(idThread);

    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
}