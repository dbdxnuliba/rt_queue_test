#include "msgc_producer_task.h"
#include <unistd.h>
#include <iostream>
pthread_t test_rtqueue_nonRTthread_id;
void* msgc_producer_task_proc(void* );
void* test_rt_queue_with_nonRTthread_fun(void* );

void  msgc_producer_init()
{
    int ret = pthread_create(&test_rtqueue_nonRTthread_id, NULL, msgc_producer_task_proc, NULL);//开始S形速度规划

    if (ret)
    {
        perror("test_rt_queue_thread_fun_thread error!");
        return ;
    }
    return;

}



void* msgc_producer_task_proc(void* )
{
    static msgC_data_related_ tmsgC_data_related_;
    static msg_data_related_ tmsg_data_related_interface;
    while(1)
    {
        usleep(10000);
        strcpy(tmsgC_data_related_.info," MSG FROM C! \n");
        tmsg_data_related_interface.msgType=MSGCTYPE;
        tmsg_data_related_interface.tmsgC=tmsgC_data_related_;
        struct msg_data_related_ *ptr = &tmsg_data_related_interface;
        int ret=enqueue(ptr,100);
        if(ret >0)
        {
            std::cout<<"c thread enqueue success"<<std::endl;
        }
        else
        {
//             std::cout<<"c thread enqueue failed"<<std::endl;
        }
    }

}
