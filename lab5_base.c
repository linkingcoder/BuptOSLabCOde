#define ORANGE_MAX_VALUE 1000000
#define APPLE_MAX_VALUE 100000000
#define K 10
#define MAX_MEASUREMENTS 200
#include <stdio.h>
#include <sys/time.h>
struct apple {
    unsigned long long a;
    unsigned long long b;
};

struct orange {
    int a[ORANGE_MAX_VALUE];
    int b[ORANGE_MAX_VALUE];
};
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
int main(int argc, const char *argv[]){
    //insert code here
    double time_use = 0;
    struct timeval start,end;
    struct  apple test;
    struct orange test1;
    int sum;
    double  K_test[K];
    for (int round = 1; round <= MAX_MEASUREMENTS; round++) {
        gettimeofday(&start, NULL);
        for (sum = 0; sum < APPLE_MAX_VALUE; ++sum) {
            test.a += sum;
            test.b += sum;
        }
        sum = 0;
        for (int index = 0; index < ORANGE_MAX_VALUE; ++index) {
            sum += test1.a[index] + test1.b[index];
        }
        gettimeofday(&end, NULL);
        time_use = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec))/1000000.0;//秒
        K_test[K - 1] = time_use;
        sort_array(K_test, K);
    }
    double average_execution_time = 0.0;
    for (int i = 0; i < K; i++) {
        average_execution_time += K_test[i];
    }
    average_execution_time /= K;
    printf("Average Execution Time of Base: %f seconds\n", average_execution_time);
    return 0;
}