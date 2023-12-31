#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
typedef struct info
{
    long int msg_type;
    int number;
    char text[BUFSIZ];
}info;
int main()
{
    int flag = 1;
    int msgid = -1;
    info data;
    long int msgtype = 0; //获取消息队列的第一个消息

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  //得到消息队列
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while (flag)    //从队列中获取消息，直到遇到 end 消息为止
    {
        if (msgrcv(msgid, (void*)&data, BUFSIZ+4, msgtype, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("no.%d receive data successfully: %s\n", data.number,data.text);
        if (strncmp(data.text, "end", 3) == 0)//遇到 end 结束
            printf("Process communication ends!");
            flag = 0;
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1)//删除消息队列
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
