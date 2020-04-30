#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void checking_mutex_action(int rc);
void * order(void *x);
const int n_cook = 6; //num of cooks
const int n_oven = 5; //num of ovens
const int t_orderlow = 1; //mins
const int t_orderhigh = 5; //mins
const int n_orderlow = 1; //pizzas
const int n_orderhigh = 5; //pizzas
const int t_prep = 1;//min for pizza preperation
const int t_bake = 10; //mins for pizza baking
unsigned int seed;