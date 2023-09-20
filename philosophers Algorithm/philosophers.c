# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

pthread_t philosopher[5];
pthread_mutex_t chopstic[5];

void *func(int n){
    int i;
    for(i=0;i<20;i++){
    printf("philosopher %d is thinking. \n", n);
    sleep(1);
    pthread_mutex_lock(&chopstic[n]);
    pthread_mutex_lock(&chopstic[(n+1)%5]);
    printf("Philosopher %d is eating Using chopstic[%d] and chopstic[%d]. \n", n+1, n, (n+1)%5);
    sleep(2);
    pthread_mutex_unlock(&chopstic[n]);
    pthread_mutex_unlock(&chopstic[(n+1)%5]);
    printf("Philosopher %d finished eating. \n", n);
    }
    return (NULL);

}

int main() {

    for (int i = 0; i < 5; i++)
    	pthread_mutex_init(&chopstic[i], NULL);

    for (int i = 0; i < 5; i++)
    	pthread_create(&philosopher[i], NULL,(void *)func, (void *)i);
    
    for(int i=0; i<5;i++)
        pthread_join(philosopher[i],NULL);

    for(int i=0; i<5;i++)
        pthread_mutex_destroy(&chopstic[i]);

    return 0;
}
