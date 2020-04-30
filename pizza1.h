#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Declaring functions
void * order(void *x);
void checking_mutex_action(int rc);

//Delcaring variables
extern const int n_cook; //num of cooks
extern const int n_oven; //num of ovens
extern const int t_orderlow; //mins
extern const int t_orderhigh; //mins
extern const int n_orderlow; //pizzas
extern const int n_orderhigh; //pizzas
extern const int t_prep; //min for pizza preperation
extern const int t_bake; //mins for pizza baking
extern unsigned int seed; //seed used for rand_r
extern int available_cooks ;
extern int available_ovens ;
extern double avg_time;
extern double max_time;

//Declaring mutexes
extern pthread_mutex_t mutex_available_cook;
extern pthread_mutex_t mutex_available_oven;
extern pthread_mutex_t mutex_avg_time;
extern pthread_mutex_t mutex_max_time;
extern pthread_mutex_t mutex_print;

//Declaring conditions
extern pthread_cond_t cond_available_cook;
extern pthread_cond_t cond_available_oven;
