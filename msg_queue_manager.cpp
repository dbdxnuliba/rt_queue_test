#include "msg_queue_manager.h"
#include <stdio.h>
#include <unistd.h>
#include <mutex>
//static queue_info_t queueInfo;
static std::queue<msg_data_related_*>  msg_to_socket_queue;
//static struct msg_data_related_ msgQueueArr[QUEUE_MEMBER_SIZE+1];
//static int messg_index =0;
pthread_mutex_t msgqueMutex;

void  queue_init()
{
    int ret =pthread_mutex_init(&msgqueMutex,NULL);
    if(ret != 0)
    {
        printf("msgqueMutex init failed\n");
    }
}

int enqueue(msg_data_related_ *ptr,uint32_t wait_usecond)
{
    msg_data_related_* _pData = new msg_data_related_;
    memcpy(_pData,ptr,sizeof(msg_data_related_));//但是对于结构体中含有指针的方式来说，此命令还是把msg_data_related_中的指针与ptr中指针指向同一块地址，所以尽量不这么干;
    //printf("will enqueue \n");
    if(get_msg_queue_size()>=QUEUE_MEMBER_SIZE)
    {
        usleep(wait_usecond);
        if(get_msg_queue_size()>=QUEUE_MEMBER_SIZE)
        {
            delete _pData;
            return -ENOMEM;
        }
        else
        {
            pthread_mutex_lock(&msgqueMutex);
            msg_to_socket_queue.push(_pData);
            pthread_mutex_unlock(&msgqueMutex);
            return 1;
        }
    }
    else
    {
        pthread_mutex_lock(&msgqueMutex);
        msg_to_socket_queue.push(_pData);
        pthread_mutex_unlock(&msgqueMutex);
        return 1;
    }

    //    messg_index++;
    //    messg_index = messg_index%QUEUE_MEMBER_SIZE;

}

int dequeue(msg_data_related_ &tmsg_data_related_,uint32_t wait_usecond)
{
    //可以用临时实例化变量作为返回值，因为发生了拷贝，但是千万别用临时指针，函数退出后临时变量发生了销毁，指向临时变量的指针成为野指针
    //msg_data_related_ new ptrTemp = NULL;
    int queue_size=get_msg_queue_size();
    if(queue_size>0)
    {
        pthread_mutex_lock(&msgqueMutex);
        tmsg_data_related_=*(msg_to_socket_queue.front());
        delete  msg_to_socket_queue.front();
        msg_to_socket_queue.pop();
        pthread_mutex_unlock(&msgqueMutex);
        return 1;
    }
    else
    {
        usleep(wait_usecond);
        if(get_msg_queue_size()<= 0)
        {
            return -ENOMEM;
        }
        else
        {
            pthread_mutex_lock(&msgqueMutex);
            tmsg_data_related_=*(msg_to_socket_queue.front());
            delete  msg_to_socket_queue.front();
            msg_to_socket_queue.pop();
            pthread_mutex_unlock(&msgqueMutex);
            return 1;
        }
    }
}

int get_msg_queue_size(void)
{
    int queue_size=0;
    pthread_mutex_lock(&msgqueMutex);
    queue_size=msg_to_socket_queue.size();
    pthread_mutex_unlock(&msgqueMutex);
    return queue_size;
}


