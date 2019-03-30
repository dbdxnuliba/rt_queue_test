
#include <rtdm/rtdm.h>
#include <native/sem.h>
#include <native/event.h>
#include <native/mutex.h>
#include <native/timer.h>
#include<native/queue.h>
#include <rtdk.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include "msga_producer_task.h"


static pro_info_t tcb;

static msg_data_related_ msgDataInterface;

static msgA_data_related_ msgAData;


void msga_producer_task_proc(void *arg);

//void *get_producer_queue(void *tcbARG)
//{
//    return &((pro_info_t *)tcbARG)->q;
//}

void * msga_producer_init()
{
    int ret =0;

    //ret = rt_queue_create(&tcb.q, "QUEUE", sizeof(struct msg_data_related_ *), QUEUE_MEMBER_SIZE, Q_FIFO);

    rt_printf("step1 \n");
    /* Creating cyclic xenomai task */
    ret = rt_task_create(&tcb.producer_task,"producerA_task",0,90,T_FPU);
    if (ret)
    {
        fprintf (stderr, "producerA_task create failed!!!!\n");
        goto release_master;
    }

    /* Starting cyclic task */
    fprintf(stdout, "starting producerA_task \n");
    ret = rt_task_start(&tcb.producer_task, &msga_producer_task_proc, NULL);
    if (ret)
    {
        fprintf (stderr, "producerA_task start failed!!!!\n");
        goto release_master;
    }
    return &tcb;
release_master:
    return NULL;
}

void msga_producer_task_proc(void *arg)
{
    int messg_index=0;
    int run=1;

    /* Set Xenomai task execution mode */
    int   ret = rt_task_set_mode(0, T_CONFORMING, NULL);
    if (ret)
    {
        printf("error while rt_task_set_mode, code %d\n",ret);
        return;
    }

    rt_task_set_periodic(NULL,TM_NOW,SM_FRAME_PERIOD_NS);
    /* Start pdo exchange loop until user stop */
    while (run)
    {
        if(rt_task_wait_period(NULL)!=0)
        {
            printf("sched_rms_period error_in_consumer_task_proc!\n");

        }

        msgAData.vel=messg_index;
        msgAData.acc=messg_index*2;
        msgAData.msg_size=msgAValiableSize+sizeof(int);

        msgDataInterface.msgType=MSGATYPE;

        msgDataInterface.tmsgA=msgAData;

        struct msg_data_related_ *ptr = &msgDataInterface;
        int ret =enqueue(ptr,100);
        if(ret >0)
        {
            std::cout<<"a thread enqueue success"<<std::endl;
        }
        else
        {
             std::cout<<"a thread enqueue failed"<<std::endl;
        }

        //        ret = rt_queue_write((RT_QUEUE *)&arg, (void *)&ptr, sizeof(struct msg_data_related_ *), Q_NORMAL);

        //        if(-ENOMEM==ret )
        //        {
        //            printf("queue no space \n");
        //            int ret_of_write=1;
        //            while(ret_of_write)
        //            {
        //                usleep(10);
        //                ret = rt_queue_write((RT_QUEUE *)&arg,&ptr , sizeof(struct msg_data_related_ *), Q_NORMAL);
        //                if(ret>=0)
        //                {
        //                    ret_of_write=0;
        //                }
        //            }
        //        }
        //        //rt_printf("producer_task_proc executing \n");

        messg_index++;
    }

    //rt_queue_delete(&tcb.q);
}



