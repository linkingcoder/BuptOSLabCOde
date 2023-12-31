#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
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
    unsigned long long b;
};

struct orange {
    int a[ORANGE_MAX_VALUE];
    int b[ORANGE_MAX_VALUE];
};

int sum = 0;  // 全局变量，线程私有

void *CalApple(void *x) {
    struct apple *apple_struct = (struct apple *)x;

    for (int i = 0; i < APPLE_MAX_VALUE; i++) {
        apple_struct->a += i;
        apple_struct->b += i;
    }

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
    pthread_t thread_apple, thread_orange;
    double  K_test[K];
    for (int round = 1; round <= MAX_MEASUREMENTS; round++) {
        gettimeofday(&start, NULL);
        // 创建线程，分别执行CalApple和CalOrange
        pthread_create(&thread_apple, NULL, CalApple, &test);
        pthread_create(&thread_orange, NULL, CalOrange, &test1);
        // 等待线程完成
        pthread_join(thread_apple, NULL);
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
