#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#ifndef PIZZA_H
#define  PIZZA_H
//Declaring functions
void * order(void *x);
void checking_mutex_action(int rc);

//Delcaring & Initializing variables
const int n_cook = 2; //num of cooks
const int n_oven = 5; //num of ovens
const int t_orderlow = 10; //mins
const int t_orderhigh = 1; //mins
const int n_orderlow = 5; //pizzas
const int n_orderhigh = 5 ;//pizzas
const int t_prep = 1; //min for pizza preperation
const int t_bake = 10; //mins for pizza baking
int t_low = 5;
int t_high = 15;
double max_cold_time = -1;
double max_delivery_time = -1;
int available_deliverer = 10;
unsigned int seed; //seed used for rand_r
int available_cooks ;
int available_ovens ;
double avg_time;
double max_time;
double avg_delivery_time;
double avg_cold_time;


//Declaring & Initializing mutexes
pthread_mutex_t mutex_available_cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_available_oven = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_print = PTHREAD_MUTEX_INITIALIZER;
//New mutexes for 2nd project:
pthread_mutex_t mutex_available_deliverer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_delivery_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_delivery_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_avg_cold_time= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_max_cold_time = PTHREAD_MUTEX_INITIALIZER;

//Defining & Initializing conditions
pthread_cond_t cond_available_cook = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_available_oven = PTHREAD_COND_INITIALIZER;
//New cond:
pthread_cond_t cond_available_deliverer = PTHREAD_COND_INITIALIZER;
#endif
