#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

sem_t x,y;
pthread_t tid;
pthread_t writerthread,readerthreads[2];
int readercount = 0;
int count = 0;

void *reader(void* param)
{
    int i=0;
    while(i<10){
    sem_wait(&x);
    readercount++;
    if(readercount==1)
        sem_wait(&y);
    sem_post(&x);
    printf("reader with tid %d is inside.\nvallue of counter is: %d\n",pthread_self(), count);
    usleep(3);
    sem_wait(&x);
    readercount--;
    if(readercount==0)
    {
        sem_post(&y);
    }
    sem_post(&x);
    printf("%d Reader is leaving\n",readercount+1);
    i++;
    usleep(15);
    }
    return NULL;
}

void *writer(void* param)
{
    //pid_t tid = syscall(__NR_gettid);
    int i=0;
    while(i<15){
    printf("Writer is in queue\n");
    sem_wait(&y);
    count++;
    printf("Writer with tid %d has entered\nchanged counter value: %d\n", pthread_self(), count);
    sem_post(&y);
    printf("Writer is leaving\n");
    i++;
    usleep(5);
    }
    return NULL;
}

int main()
{
    int i;
    sem_init(&x,0,1);
    sem_init(&y,0,1);
    pthread_create(&writerthread, NULL, writer, NULL);
    usleep(30);
    for(i=0;i<2;i++)
    {
        pthread_create(&readerthreads[i],NULL,reader,NULL);
    }

    pthread_join(writerthread,NULL);

    for(i=0;i<2;i++)
    {
        pthread_join(readerthreads[i],NULL);
    }

    return 0;
}

