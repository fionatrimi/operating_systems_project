#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "pizza1.h"

//Initializing variables
const int n_cook = 2;
const int n_oven = 5;
const int n_deliverer = 10;
const int t_orderlow = 1;
const int t_orderhigh = 5;
const int n_orderlow = 1;
const int n_orderhigh = 5;
const int t_prep = 1;
const int t_bake = 10;
const int t_low = 5;
const int t_high = 15;
double avg_delivery_time ;
double avg_cold_time;
double max_cold_time = -1;
double max_delivery_time = -1 ;
unsigned int seed;
int available_cooks;
int available_ovens;
int available_deliverer;

//Initializing mutexes
pthread_mutex_t mutex_available_cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_available_oven = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_available_deliverer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_delivery_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_delivery_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_cold_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_cold_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;

//Initializing conditions
pthread_cond_t cond_available_cook = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_oven = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_deliverer = PTHREAD_COND_INITIALIZER;

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
  struct timespec order_time_begin;
  struct timespec order_time_end;

  struct timespec cold_time_begin;
  struct timespec cold_time_end;
  
  struct timespec delivery_time_begin;
  struct timespec delivery_time_end;

  clock_gettime(CLOCK_REALTIME, &order_time_begin);

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

  //Locking cook to be available again
  rc = pthread_mutex_lock(&mutex_available_cook);
  checking_mutex_action(rc);
  ++available_cooks; // Increase cooks by 1
  pthread_cond_signal(&cond_available_cook);
  rc = pthread_mutex_unlock(&mutex_available_cook);
  checking_mutex_action(rc);

  sleep(t_bake); //Waiting for pizzas to bake

  clock_gettime(CLOCK_REALTIME, &cold_time_begin);

  //ΠΕΡΙΜΕΝΕΙ ΓΙΑ ΔΙΑΘΕΣΙΜΟ ΔΙΑΝΟΜΕΑ
  rc = pthread_mutex_lock(&mutex_available_deliverer);
  checking_mutex_action(rc);
  while (available_deliverer<=0){
    pthread_cond_wait(&cond_available_deliverer, &mutex_available_deliverer);
  }

  available_deliverer--; //Decreasing available cooks by 1
  rc = pthread_mutex_unlock(&mutex_available_deliverer);  //Cook unlocked
  checking_mutex_action(rc);

  //Locking oven to be available again
  rc = pthread_mutex_lock(&mutex_available_oven);
  checking_mutex_action(rc);
  ++available_ovens; // Increase ovens by 1
  pthread_cond_signal(&cond_available_oven);
  rc = pthread_mutex_unlock(&mutex_available_oven); //Oven unlocked
  checking_mutex_action(rc);

  // ΧΡΟΝΟΣ ΠΑΡΑΔΟΣΗΣ
  clock_gettime(CLOCK_REALTIME, &delivery_time_begin);

  int random_delivery_time = rand_r(&seed) % t_high + t_low;
  sleep(random_delivery_time);

  clock_gettime(CLOCK_REALTIME, &cold_time_end);

  clock_gettime(CLOCK_REALTIME, &delivery_time_end);
  sleep(random_delivery_time);
  // ΑΠΕΛΕΥΘΕΡΩΝΩ ΤΟΥΣ ΔΙΑΝΟΜΕΙΣ
  rc = pthread_mutex_lock(&mutex_available_deliverer);
  checking_mutex_action(rc);
  ++available_deliverer; // Increase ovens by 1
  pthread_cond_signal(&cond_available_deliverer);
  rc = pthread_mutex_unlock(&mutex_available_deliverer); //Oven unlocked
  checking_mutex_action(rc);

  // Ending timer for this order
  clock_gettime(CLOCK_REALTIME, &order_time_end);

  //Locking/Unlocking output
  rc = pthread_mutex_lock(&mutex_print);
  checking_mutex_action(rc);
  printf("Order with ID %d was ready in %d minutes and it was cold for %d minutes.\n", id, (int)(order_time_end.tv_sec-order_time_begin.tv_sec),(int)(cold_time_end.tv_sec-cold_time_begin.tv_sec));
  rc = pthread_mutex_unlock(&mutex_print);
  checking_mutex_action(rc);

  //YPOLOGISMOS MESHS TIMHS KRYWMATOS
  rc = pthread_mutex_lock(&mutex_avg_cold_time);
  checking_mutex_action(rc);
  avg_cold_time += cold_time_end.tv_sec - cold_time_begin.tv_sec;
  rc = pthread_mutex_unlock(&mutex_avg_cold_time);
  checking_mutex_action(rc);

  //YPOLOGISMOS MAX TIMHS KRYWMATOS
  rc = pthread_mutex_lock(&mutex_max_cold_time);
  checking_mutex_action(rc);
  if (cold_time_end.tv_sec - cold_time_begin.tv_sec > max_cold_time){
        max_cold_time = cold_time_end.tv_sec - cold_time_begin.tv_sec;
  }
  rc = pthread_mutex_unlock(&mutex_max_cold_time);
  checking_mutex_action(rc);

  //YPOLOGISMOS MESHS TIMHS PARADOSHS
  rc = pthread_mutex_lock(&mutex_avg_delivery_time);
  checking_mutex_action(rc);
  avg_delivery_time += delivery_time_end.tv_sec - delivery_time_begin.tv_sec;
  rc = pthread_mutex_unlock(&mutex_avg_delivery_time);
  checking_mutex_action(rc);

  //YPOLOGISMOS MAX TIMHS PARADOSHS
  rc = pthread_mutex_lock(&mutex_max_delivery_time);
  checking_mutex_action(rc);
  if (delivery_time_end.tv_sec - delivery_time_begin.tv_sec > max_delivery_time){
        max_delivery_time = delivery_time_end.tv_sec - delivery_time_begin.tv_sec;
  }
  rc = pthread_mutex_unlock(&mutex_max_delivery_time);
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
  available_deliverer = n_deliverer;

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
  printf("\nMax delivery time: %f minutes.\nAverage delivery time: %f minutes.\n",max_delivery_time,avg_delivery_time/n_cust);
  printf("\nMax cold time: %f minutes.\nAverage cold time: %f minutes.\n",max_cold_time,avg_cold_time/n_cust);

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
