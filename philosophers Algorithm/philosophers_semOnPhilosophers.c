#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
 
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+1) % 5
#define RIGHT (ph_num+4) % 5
 
sem_t mutex;
sem_t sem[5];
 
void * philospher(void *num);
void take_fork(int);
void put_fork(int);
void test(int);
 
int state[5];
int phil_num[5]={0,1,2,3,4};
 
int main()
{
    int i;
    pthread_t thread_id[5];
    sem_init(&mutex, 0, 1);
    for(i=0;i<5;i++)
        sem_init(&sem[i], 0, 0);
    for(i=0;i<5;i++)
    {
        pthread_create(&thread_id[i], NULL, philospher, &phil_num[i]);
        printf("Philosopher %d is thinking\n", i+1);
    }
    for(i=0;i<5;i++)
        pthread_join(thread_id[i], NULL);
}
 
void *philospher(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}
 
void take_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = HUNGRY;
    printf("Philosopher %d is Hungry\n", ph_num+1);
    test(ph_num);
    sem_post(&mutex);
    sem_wait(&sem[ph_num]);
    sleep(1);
}
 
void test(int ph_num)
{
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[ph_num] = EATING;
        sleep(2);
        printf("Philosopher %d takes fork %d and %d\n", ph_num+1, ph_num, (ph_num+1)%5);
        printf("Philosopher %d is Eatingn\n", ph_num+1);
        sem_post(&sem[ph_num]);
    }
}
 
void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n", ph_num+1, ph_num, (ph_num+1)%5);
    printf("Philosopher %d is thinking\n", ph_num+1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}