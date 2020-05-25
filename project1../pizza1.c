#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "pizza1.h"

//Initializing variables
const int n_cook = 6;
const int n_oven = 5;
const int t_orderlow = 1;
const int t_orderhigh = 5;
const int n_orderlow = 1;
const int n_orderhigh = 5;
const int t_prep = 1;
const int t_bake = 10;
double avg_time ;
double max_time = -1 ;
unsigned int seed;
int available_cooks;
int available_ovens;

//Initializing mutexes
pthread_mutex_t mutex_available_cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_available_oven = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;

//Initializing conditions
pthread_cond_t cond_available_cook = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_oven = PTHREAD_COND_INITIALIZER;

void checking_mutex_action(int rc){
        //In case a mutex fails the program exits
        if (rc != 0) {
                printf("ERROR: RETURN CODE IS %d\n", rc);
        exit(-1);
    }
}

void * order(void *order_id){
  int rc;
  int id = *(int*)order_id;

  //Staring timer here
  struct timespec begin;
  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &begin);

  //Working with cook
  //(lock and unlock them when done with an order)
  rc = pthread_mutex_lock(&mutex_available_cook);
  checking_mutex_action(rc);
  while (available_cooks<=0){
    pthread_cond_wait(&cond_available_cook, &mutex_available_cook);
  }

  available_cooks--; //Decreasing available cooks by 1
  rc = pthread_mutex_unlock(&mutex_available_cook);  //Cook unlocked
  checking_mutex_action(rc);

  //Preparing every pizza of order for 10 mins
  //Calculating how many pizzas for an order
  int pizzas = rand_r(&seed) % n_orderhigh + n_orderlow;
  sleep(t_prep*pizzas);

  //Working with oven
  //(lock and unlock them when done with baking)
  rc = pthread_mutex_lock(&mutex_available_oven);
  checking_mutex_action(rc);
  while (available_ovens <= 0){
    pthread_cond_wait(&cond_available_oven, &mutex_available_oven);
  }
  available_ovens--; //Make one more oven unavailable
  rc = pthread_mutex_unlock(&mutex_available_oven); //Oven unlocked
  checking_mutex_action(rc);

  sleep(t_bake); //Waiting for pizzas to bake

  //Locking cook to be available again
  rc = pthread_mutex_lock(&mutex_available_cook);
  checking_mutex_action(rc);
  ++available_cooks; // Increase cooks by 1
  pthread_cond_signal(&cond_available_cook);
  rc = pthread_mutex_unlock(&mutex_available_cook);
  checking_mutex_action(rc);

  //Locking oven to be available again
  rc = pthread_mutex_lock(&mutex_available_oven);
  checking_mutex_action(rc);
  ++available_ovens; // Increase ovens by 1
  pthread_cond_signal(&cond_available_oven);
  rc = pthread_mutex_unlock(&mutex_available_oven); //Oven unlocked
  checking_mutex_action(rc);

  // Ending timer for this order
  clock_gettime(CLOCK_REALTIME, &end);

  //Locking/Unlocking output
  rc = pthread_mutex_lock(&mutex_print);
  checking_mutex_action(rc);
  printf("Order with ID %d was ready in %d minutes.\n", id, (int)(end.tv_sec-begin.tv_sec));
  rc = pthread_mutex_unlock(&mutex_print);
  checking_mutex_action(rc);

  //Locking/Unlocking the variable to update the total time
  rc = pthread_mutex_lock(&mutex_avg_time);
  checking_mutex_action(rc);
  avg_time+=end.tv_sec-begin.tv_sec;
  rc = pthread_mutex_unlock(&mutex_avg_time);
  checking_mutex_action(rc);

  //Locking/Unlocking the variable to update the maximum time
  rc = pthread_mutex_lock(&mutex_max_time);
  checking_mutex_action(rc);
  if (end.tv_sec-begin.tv_sec > max_time){
        max_time = end.tv_sec-begin.tv_sec;
  }
  rc = pthread_mutex_unlock(&mutex_max_time);
  checking_mutex_action(rc);

  pthread_exit(NULL);
  return 0;
}

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
  printf("Number of customers: %d\nSeed: %d\n\n",n_cust, seed);
  available_cooks = n_cook;
  available_ovens = n_oven;

  //Creting n_cust pthreads giving each one of them an id
  //Every order given by customers is a thread
  pthread_t threads[n_cust];
  int id[n_cust];
  for (int i=0; i<n_cust; i++){
    if(i>0){
        sleep(rand_r(&seed) % t_orderhigh + t_orderlow);
    }
    id[i] = i+1;
    pthread_create(&threads[i], NULL, &order, &id[i]);
  }


  //Waiting all threads to execute
  for (int i=0; i<n_cust; i++){
    pthread_join(threads[i], NULL);
  }

  //Printing final output
  printf("\nMax order time: %f minutes.\nAverage time: %f minutes.\n",max_time,avg_time/n_cust);

  //Destroying pthreads' mutexes and conditions
  pthread_mutex_destroy(&mutex_available_cook);
  pthread_mutex_destroy(&mutex_available_oven);
  pthread_mutex_destroy(&mutex_max_time);
  pthread_mutex_destroy(&mutex_avg_time);
  pthread_mutex_destroy(&mutex_print);
  pthread_cond_destroy(&cond_available_cook);
  pthread_cond_destroy(&cond_available_oven);
        
  return 0;
}
