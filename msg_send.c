#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#define MAX_TEXT 512
typedef struct info
{
    long int msg_type;
    int number;
    char text[MAX_TEXT];
}info;

int main()
{
    int flag = 1;
    int i = 0;
    info data;
    char buffer[BUFSIZ];
    int msgid = -1;

    msgid = msgget((key_t)1234, 0666| IPC_CREAT);   //建立消息队列,返回队列标识符
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while (flag)    //向消息队列中写消息，直到写入 end
    {
        i++;
        printf("The message  to send: ");//输入转杯传送的数据数据
        fgets(buffer, BUFSIZ, stdin);
        data.msg_type = 1; //消息类型设置为1
        data.number = i;
        strcpy(data.text, buffer);

        if (msgsnd(msgid, (void*)&data, MAX_TEXT+4, 0) == -1)       //向队列发送数据
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        printf("no.%d send message successfully\n", data.number);
        if (strncmp(buffer, "end", 3) == 0)//输入 end 结束输入
            flag = 0;
        sleep(1);//进程休眠，否则输出错位#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#define MAX_TEXT 512
        typedef struct info
        {
            long int msg_type;
            int number;
            char text[MAX_TEXT];
        }info;

        int main()
        {
            int flag = 1;
            int i = 0;
            info data;
            char buffer[BUFSIZ];
            int msgid = -1;

            msgid = msgget((key_t)1234, 0666| IPC_CREAT);   //建立消息队列,返回队列标识符
            if (msgid == -1)
            {
                fprintf(stderr, "msgget failed with error: %d\n", errno);
                exit(EXIT_FAILURE);
            }

            while (flag)    //向消息队列中写消息，直到写入 end
            {
                i++;
                printf("The message  to send: ");//输入转杯传送的数据数据
                fgets(buffer, BUFSIZ, stdin);
                data.msg_type = 1; //消息类型设置为1
                data.number = i;
                strcpy(data.text, buffer);

                if (msgsnd(msgid, (void*)&data, MAX_TEXT+4, 0) == -1)       //向队列发送数据
                {
                    fprintf(stderr, "msgsnd failed\n");
                    exit(EXIT_FAILURE);
                }
                printf("no.%d send message successfully\n", data.number);
                if (strncmp(buffer, "end", 3) == 0)//输入 end 结束输入
                    flag = 0;
                sleep(1);//进程休眠，否则输出错位
            }
            exit(EXIT_SUCCESS);
        }

    }
    exit(EXIT_SUCCESS);
}
