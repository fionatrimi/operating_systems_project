#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void * order(void *x);
int n_cook = 6; //num of cooks
int n_oven = 5; //num of ovens
int t_orderlow = 1; //mins
int t_orderhigh = 5; //mins
int n_orderlow = 1; //pizzas
int n_orderhigh = 5; //pizzas
int t_prep = 1;//min for pizza preperation
int t_bake = 10; //mins for pizza baking
unsigned int seed;

//Variables
int available_cooks ;
int available_ovens ;
double avg_time ;
double max_time;


//Mutexes
pthread_mutex_t mutex_available_cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_available_oven = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;






//Conditions
pthread_cond_t cond_available_cook = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_oven = PTHREAD_COND_INITIALIZER;



int main(int argc, char * argv[]){

  /*
    * Checking if programm args are valid, 
      prints the needed messages, 
      sets the values to n_cust and seed.
    * seed gets value 0 if invalid arguments is given (ex. string)
	  */
  if (argc!=3){
    printf("Args must be two (number of customers(int>0) and seed(int))\n");
    return -1;
  }
  int n_cust = atoi(argv[1]);
  seed = atoi(argv[2]);
  if (n_cust<=0){
    printf("Invalid args (number of customers must be int > 0)\n");
    return -1;
  }
  printf("Number of customers: %d\nSeed: %d\n",n_cust, seed);
  available_cooks = n_cook;
  available_ovens = n_oven;

  /*
    * Creting n_cust pthreads giving each one of them an id
    * Every order given by customers is a thread
  */
  pthread_t threads[n_cust];
  int id[n_cust];
  for (int i=0; i<n_cust; i++){
    id[i] = i+1;
    printf("Main: (creating thread) incoming order with id: %d\n", i+1);
    pthread_create(&threads[i], NULL, &order, &id[i]);
  }


  /*
    Waiting all threads to execute
  */
  for (int i=0; i<n_cust; i++){
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex_available_cook);
  pthread_mutex_destroy(&mutex_available_oven);
  pthread_mutex_destroy(&mutex_max_time);
  pthread_mutex_destroy(&mutex_avg_time);
  pthread_mutex_destroy(&mutex_print);
  pthread_cond_destroy(&cond_available_cook);
  pthread_cond_destroy(&cond_available_oven);
  return 0;

}

void * order(void *order_id){
  int rc;
  int id = *(int*)order_id;
  //struct timespec cook_start_time;

  printf("Hello from order: %d\n", id);
  //start timer here
  //clock_gettime(CLOCK_REALTIME, &cook_start_time);
  rc = pthread_mutex_lock(&mutex_available_cook);

  while (available_cooks<=0){
    printf("No cook/oven available. Order %d Blocked.\n", id);
    pthread_cond_wait(&cond_available_cook, &mutex_available_cook);
  }

  printf("Order %d is executing\n", id);
  available_cooks--;

  rc = pthread_mutex_unlock(&mutex_available_cook);

  int pizzas = rand_r(&seed) % n_orderhigh + n_orderlow;
  sleep(t_prep*pizzas);


  rc = pthread_mutex_lock(&mutex_available_oven);
  while (available_ovens <= 0){
    printf("No cook/oven available. Order %d Blocked.\n", id);
    pthread_cond_wait(&cond_available_oven, &mutex_available_oven);
  }
  available_ovens--;
  rc = pthread_mutex_unlock(&mutex_available_oven);
  sleep(t_bake);//sleep(t_bake);

  rc = pthread_mutex_lock(&mutex_available_cook);
  ++available_cooks;
  pthread_cond_signal(&cond_available_cook);
  rc = pthread_mutex_unlock(&mutex_available_cook);

  rc = pthread_mutex_lock(&mutex_available_oven);
  ++available_ovens;
  pthread_cond_signal(&cond_available_oven);
  rc = pthread_mutex_unlock(&mutex_available_oven);

  rc = pthread_mutex_lock(&mutex_print);

  printf("Successfull execution of order %d\n", id);

  rc = pthread_mutex_unlock(&mutex_print);
  //end timer here

  pthread_exit(NULL);



  /*int rc;
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
    
}
