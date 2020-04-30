#include <project1.h>

//Variables
int available_cooks ;
int available_ovens ;
double avg_time ;
double max_time = -1 ;


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
	int rc;
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
    if(i>0){
        sleep(rand_r(&seed) % t_orderhigh + t_orderlow);
    }
    id[i] = i+1;
    printf("Main: (creating thread) incoming order with id: %d\n", i+1);
    rc=pthread_create(&threads[i], NULL, &order, &id[i]);
    checking_mutex_action(rc);
  }


  /*
    Waiting all threads to execute
  */
  for (int i=0; i<n_cust; i++){
    rc = pthread_join(threads[i], NULL);
    checking_mutex_action(rc);
  }
  printf("max time %f avg time %f",max_time,avg_time/n_cust);

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
  struct timespec begin;
  struct timespec end;

  printf("Hello from order: %d\n", id);
  //start timer here
  clock_gettime(CLOCK_REALTIME, &begin);
  rc = pthread_mutex_lock(&mutex_available_cook);
  checking_mutex_action(rc);
  while (available_cooks<=0){
    printf("No cook/oven available. Order %d Blocked.\n", id);
    pthread_cond_wait(&cond_available_cook, &mutex_available_cook);
  }

  printf("Order %d is executing\n", id);
  available_cooks--;

  rc = pthread_mutex_unlock(&mutex_available_cook);
  checking_mutex_action(rc);

  int pizzas = rand_r(&seed) % n_orderhigh + n_orderlow;
  sleep(t_prep*pizzas);


  rc = pthread_mutex_lock(&mutex_available_oven);
  checking_mutex_action(rc);
  while (available_ovens <= 0){
    printf("No cook/oven available. Order %d Blocked.\n", id);
    pthread_cond_wait(&cond_available_oven, &mutex_available_oven);
  }
  available_ovens--;
  rc = pthread_mutex_unlock(&mutex_available_oven);
  checking_mutex_action(rc);
  sleep(t_bake);//sleep(t_bake);

  rc = pthread_mutex_lock(&mutex_available_cook);
  checking_mutex_action(rc);
  ++available_cooks;
  pthread_cond_signal(&cond_available_cook);
  rc = pthread_mutex_unlock(&mutex_available_cook);
  checking_mutex_action(rc);

  rc = pthread_mutex_lock(&mutex_available_oven);
  checking_mutex_action(rc);
  ++available_ovens;
  pthread_cond_signal(&cond_available_oven);
  rc = pthread_mutex_unlock(&mutex_available_oven);
  checking_mutex_action(rc);
  clock_gettime(CLOCK_REALTIME, &end);
  rc = pthread_mutex_lock(&mutex_print);
  checking_mutex_action(rc);

  printf("Successfull execution of order %d in time %d \n pizzas %d", id, (int)(end.tv_sec-begin.tv_sec),pizzas);
  rc = pthread_mutex_unlock(&mutex_print);
  checking_mutex_action(rc);

  rc = pthread_mutex_lock(&mutex_avg_time);
  checking_mutex_action(rc);
  avg_time+=end.tv_sec-begin.tv_sec;
  rc = pthread_mutex_unlock(&mutex_avg_time);
  checking_mutex_action(rc);

  rc = pthread_mutex_lock(&mutex_max_time);
  checking_mutex_action(rc);
  if (end.tv_sec-begin.tv_sec > max_time){
  	max_time = end.tv_sec-begin.tv_sec;
  }
  rc = pthread_mutex_unlock(&mutex_max_time);
  checking_mutex_action(rc);
  //end timer here

  pthread_exit(NULL);

}
void checking_mutex_action(int rc){
	if (rc != 0) {
		printf("ERROR: return code is %d\n", rc);
       	exit(-1);
    }
}
