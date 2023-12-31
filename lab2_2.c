#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
static int a = 5;
typedef struct member
{
    int a;
    char* s;
}member;
void* sendChar(char* arg)
{
    char* str;
    str = arg;
    printf("=========\n");
    printf("now running thread sendChar\n");
    printf("The parameter passed from main is %s\n", str);
    return (void*)0;
}
void* sendStruct(void* arg)
{
    member* recv;
    member* send;
    recv = (member*)arg;
    printf("=========\n");
    printf("now running thread sendStruct\n");
    printf("menber->a = %d\n", recv->a);
    printf("menber->s = %s\n", recv->s);
    send = (member*)malloc(sizeof(member));
    send->a = recv->a * 2;
    send->s = "Jack";
    return (void*)send;
}
void* getData(void* arg)
{
    printf("=========\n");
    printf("now running thread getData\n");
    printf("New pthread...\n");
    printf("a = %d\n", a);
    a = 3;
    return (void*)0;
}
int main()
{
    int error;
    pthread_t id1, id2, id3;
    char* str1 = "Hello!";
    char* attr = str1;
    error = pthread_create(&id1, NULL, sendChar, (void*)attr);
    if (error != 0)
    {
        printf("This pthread is not created!\n");
        return -1;
    }
    sleep(1);
    printf("pthread is created..\n");

    member* p;
    void* get;
    p = (member*)malloc(sizeof(member));
    p->a = 1;
    p->s = "Robben!";
    error = pthread_create(&id2, NULL, sendStruct, (void*)p);
    if (error)
    {
        printf("pthread is not created!\n");
        return -1;
    }
    sleep(1);
    printf("pthread is created!\n");
    pthread_join(id2, &get);
    printf("member->a=%d\n", ((member*)get)->a);
    printf("member->s=%s\n", ((member*)get)->s);
    free(p);
    p = NULL;

    error = pthread_create(&id3, NULL, getData, NULL);
    if (error != 0)
    {
        printf("new thread is not created!\n");
        return -1;
    }
    sleep(1);
    printf("New thread is created...\n");#include <stdio.h>
                                          #include <pthread.h>
                                          #include <stdlib.h>
    static int a = 5;
    typedef struct member
    {
        int a;
        char* s;
    }member;
    void* sendChar(char* arg)
    {
        char* str;
        str = arg;
        printf("=========\n");
        printf("now running thread sendChar\n");
        printf("The parameter passed from main is %s\n", str);
        return (void*)0;
    }
    void* sendStruct(void* arg)
    {
        member* recv;
        member* send;
        recv = (member*)arg;
        printf("=========\n");
        printf("now running thread sendStruct\n");
        printf("menber->a = %d\n", recv->a);
        printf("menber->s = %s\n", recv->s);
        send = (member*)malloc(sizeof(member));
        send->a = recv->a * 2;
        send->s = "Jack";
        return (void*)send;
    }
    void* getData(void* arg)
    {
        printf("=========\n");
        printf("now running thread getData\n");
        printf("New pthread...\n");
        printf("a = %d\n", a);
        a = 3;
        return (void*)0;
    }
    int main()
    {
        int error;
        pthread_t id1, id2, id3;
        char* str1 = "Hello!";
        char* attr = str1;
        error = pthread_create(&id1, NULL, sendChar, (void*)attr);
        if (error != 0)
        {
            printf("This pthread is not created!\n");
            return -1;
        }
        sleep(1);
        printf("pthread is created..\n");

        member* p;
        void* get;
        p = (member*)malloc(sizeof(member));
        p->a = 1;
        p->s = "Robben!";
        error = pthread_create(&id2, NULL, sendStruct, (void*)p);
        if (error)
        {
            printf("pthread is not created!\n");
            return -1;
        }
        sleep(1);
        printf("pthread is created!\n");
        pthread_join(id2, &get);
        printf("member->a=%d\n", ((member*)get)->a);
        printf("member->s=%s\n", ((member*)get)->s);
        free(p);
        p = NULL;

        error = pthread_create(&id3, NULL, getData, NULL);
        if (error != 0)
        {
            printf("new thread is not created!\n");
            return -1;
        }
        sleep(1);
        printf("New thread is created...\n");
        printf("the data change into %d\n", a);
        return 0;
    }
    printf("the data change into %d\n", a);
    return 0;
}