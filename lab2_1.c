#include<pthread.h>
#include<stdio.h>
#include <stdio.h>
#include <sys/select.h>
static void sleep_ms(unsigned int secs)//实现ms级别的sleep
{
    struct timeval tval;
    tval.tv_sec = secs / 1000;
    tval.tv_usec = (secs * 1000) % 1000000;
    select(0, NULL, NULL, NULL, &tval);
}
void* thread_1(void* arg) {//exit退出
    int i;
    for (i = 0; i < 4; i++)
    {
        printf("now running thread [1] ID:%d\n", (unsigned int)pthread_self());
        sleep_ms(50);
    }
    pthread_exit((void*)1);
}

void* thread_2(void* arg) {//正常执行完毕退出
    int j;
    for (j = 0; j < 4; j++)
    {
        sleep_ms(345);
        printf("now running thread [2] ID:%d\n", (unsigned int)pthread_self());

    }
    return 2;
}
void* thread_3(void* arg) {//cancel
    int k;
    for (k = 0; k < 4; k++)
    {
        printf("now running thread [3] ID:%d\n", (unsigned int)pthread_self());
        sleep_ms(789);
    }
}

int main() {
    int res = 0;
    void* code1, * code2, * code3;
    pthread_t thread1, thread2, thread3;
    /*线程1pthread_exit*/
    res = pthread_create(&thread1, NULL, (void*)thread_1, NULL);
    if (res != 0) {
        printf("create thread 1 failed\n");
        return -1;
    }
    res = pthread_join(thread1, &code1);
    if (res) {
        printf("thread 1 is not exit\n ");
        return -2;
    }
    printf("The exit of thread 1 is %d\n", (int)code1);

    /*线程2 正常return*/
    res = pthread_create(&thread2, NULL, (void*)thread_2, NULL);
    if (res != 0) {
        printf("create thread 2 failed\n");
        return -1;
    }
    pthread_join(thread2, &code2);
    if (res) {
        printf("thread 2 is not exit\n ");
        return -2;
    }
    printf("The exit of thread 2 is %d\n", (int)code2);

    /*线程3 cancel被迫中止*/
    res = pthread_create(&thread3, NULL, (void*)thread_3, NULL);
    if (res != 0) {
        printf("create thread 2 failed\n");
        return -1;
    }
    sleep_ms(50);
    res = pthread_cancel(thread3);
    if (res != 0) {
        printf("fail to cancel thread3\n");
    }
    res = pthread_join(thread3, &code3);
    if (res != 0) {
        printf("fail to join thread3\n");
        return 0;
    }
    if (code3 == PTHREAD_CANCELED) {
        printf("thread3 is canceled\n");
        printf("The exit of thread 3 is %d\n", (int)code3);
    }
    else {
        printf("error\n");
    }

    return 0;
}