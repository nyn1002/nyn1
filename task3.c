#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define N 100
#define producerNum  15
#define consumerNum  5
#define true 1


int buffer[N] = {0};
int in = 0;
int out = 0;
int proCount = 0;  //生产者生产的物资
int mutex = 1, empty = N, full=0, proCmutex = 1;  //mutex互斥锁

void * producer(void * a){
    while(true){
        while(proCmutex <= 0);
        proCmutex--;
        proCount++;
        printf("生产者，消息ID=%d, 在缓冲池中的位置=%d\n",proCount,in);
        proCmutex++;

        while(empty <= 0){
            printf("缓冲区已满！\n");
        }
        empty--;

        while(mutex <= 0);
        mutex--;

        buffer[in] = proCount;
        in = (in + 1) % N;

        mutex++;
        full++;
        sleep(5);
    }
}

void * consumer(void *b){
    while(true){
        while(full <= 0){
            printf("缓冲区为空！\n");
        }
        full--;

        while(mutex <= 0);
        mutex--;

        int nextCount = buffer[out];
        buffer[out] = 0;//消费完将缓冲区设置为0

        out = (out + 1) % N;

        mutex++;
        empty++;

        printf("\t\t\t\t消费者，使用消息ID=%d，在缓冲池中的位置=%d\n", nextCount,out);
        sleep(5);
    }
}

int main()
{
    pthread_t threadPool[producerNum+consumerNum];
    int i;
    for(i = 0; i < producerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, producer, NULL) == -1){
            printf("ERROR, fail to create producer%d\n", i);
            exit(1);
        }
        threadPool[i] = temp;
    }//创建生产者进程放入线程池

	//创建消费者进程放入线程池
    for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, consumer, NULL) == -1){
            printf("ERROR, fail to create consumer%d\n", i);
            exit(1);
        }
        threadPool[i+producerNum] = temp;
    }

    void * result;
    for(i = 0; i < producerNum+consumerNum; i++){
        if(pthread_join(threadPool[i], &result) == -1){
            printf("fail to recollect\n");
            exit(1);
        }
    }//运行线程池
    return 0;
}