#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#define ORANGE_MAX_VALUE 1000000
#define APPLE_MAX_VALUE 100000000
#define K 10
#define MAX_MEASUREMENTS 200

// 排序数组
void sort_array(double *array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // 交换元素
                double temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}
struct apple {
    unsigned long long a;
    char c[128];//cache优化
    unsigned long long b;
};

struct orange {
    int a[ORANGE_MAX_VALUE];
    int b[ORANGE_MAX_VALUE];
};

int sum = 0;  // 全局变量
sem_t mutex_apple;//控制互斥访问apple
void *CalAinApple(void *a) {
    sem_wait(&mutex_apple);
    struct apple *apple_struct = (struct apple *)a;
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(0, &cpu_set);
    if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) == -1) {
        perror("sched_setaffinity");
    }
    for (int i = 0; i < APPLE_MAX_VALUE; i++) {
        apple_struct->a += i;
    }
    sem_post(&mutex_apple);
    return NULL;
}
void *CalBinApple(void *b) {
    sem_wait(&mutex_apple);
    struct apple *apple_struct = (struct apple *)b;
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(1, &cpu_set);
    if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) == -1) {
        perror("sched_setaffinity");
    }
    for (int i = 0; i < APPLE_MAX_VALUE; i++) {
        apple_struct->b += i;
    }
    sem_post(&mutex_apple);
    return NULL;
}

void *CalOrange(void *x) {
    struct orange *orange_struct = (struct orange *)x;
    for (int index = 0; index < ORANGE_MAX_VALUE; ++index) {
        sum += orange_struct->a[index] + orange_struct->b[index];
    }
    return NULL;
}

int main() {
    double time_use = 0;
    struct timeval start,end;
    struct apple test;
    struct orange test1;
    pthread_t thread_appleA,thread_appleB, thread_orange;
    sem_init(&mutex_apple,0,1);
    double  K_test[K];
    for (int round = 1; round <= MAX_MEASUREMENTS; round++) {
        // 创建线程，分别执行CalApple和CalOrange
        pthread_create(&thread_appleA, NULL, CalAinApple, &test);
        pthread_create(&thread_appleB, NULL, CalBinApple, &test);
        pthread_create(&thread_orange, NULL, CalOrange, &test1);
        gettimeofday(&start, NULL);
        // 等待线程完成
        pthread_join(thread_appleA, NULL);
        pthread_join(thread_appleB, NULL);
        pthread_join(thread_orange, NULL);
        gettimeofday(&end, NULL);
        time_use = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec))/1000000.0;//微秒
        K_test[K - 1] = time_use;
        sort_array(K_test, K);
    }
    double average_execution_time = 0.0;
    for (int i = 0; i < K; i++) {
        average_execution_time += K_test[i];
    }
    average_execution_time /= K;
    printf("Average Execution Time: %f seconds\n", average_execution_time);
    return 0;
}
