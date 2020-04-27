#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
/*#define N 10
pthread_mutex_t lock;
pthread_cond_t cond;
int cooks=2;
int id[N];
*/

void * order(void *x);
int n_cook = 6; //num of cooks
int n_oven = 5; //num of ovens
int t_orderlow = 1; //mins
int t_orderhigh = 5; //mins
int n_orderlow = 1; //pizzas
int n_orderhigh = 5; //pizzas
int t_prep = 1;//min for pizza preperation
int t_bake = 10; //mins for pizza baking
//int id[n_cust]

int main(){
  //make this program args:
  int n_cust = 5;
  int seed = 1;

  pthread_t threads[n_cust];
  int id[n_cust];
  for (int i=0; i<n_cust; i++){
    id[i] =i+1;
    printf("Main: creating thread %d\n", i+1);
    rc = pthread_create(&threads[i], NULL, order, &id[i]);
  }



  /*
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
*/

void * order(void *x){
  /*int id = *(int*)x;
  int rc;
  printf("Hello from order: %d\n", id);
  rc = pthread_mutex_lock(&lock);

  while (cooks==0){
    printf("H paraggelia %d den brike paraskevasth. Blocked...\n", id);
    rc= pthread_cond_wait(&cond, &lock);
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
  */
    pthread_exit(NULL);
}
