#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 
#define N 10
pthread_mutex_t lock;
pthread_cond_t cond;
int cooks=2;
int id[N];
void * order(void *x);

int main(){
  int rc;
  pthread_t threads[N];
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);

  for(int i=0; i<N; i++){
    id[i] = i+1;
    printf("Main: creating thread %d\n", i+1);
    rc = pthread_create(&threads[i], NULL, order, &id[i]);
  }

  for (int i=0; i<N; i++){
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  return 0;
}

void * order(void *x){
  int id = *(int*)x;
  int rc;
  printf("Hello from order: %d\n", id);
  rc = pthread_mutex_lock(&lock);

  while (cooks==0){
    printf("H paraggelia %d den brike paraskevasth. Blocked...\n", id);
    rc= pthread_cond_wait(&cond, &block);
  }

  printf("H paraggelia %d eksipiretitai.\n", id);
  cooks--;
  rc = pthread_mutex_unlock(&lock);
  sleep(5);
  rc=pthread_mutex_lock(&lock);
  printf("H paraggelia %d eksipiretithike epitixos! \n", id);
  cooks++;
  rc = pthread_cond_signal(&cond);
  rc = pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}
