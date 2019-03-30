#include "msgb_producer_task.h"
#include <rtdm/rtdm.h>
#include <native/task.h>
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

static pro_info_t tcb;
static msg_data_related_ msgDataInterface;
static msgB_data_related_ msgBData;

void msgb_producer_task_proc(void *arg);
void * msgb_producer_init()
{
    int ret =0;

    //ret = rt_queue_create(&tcb.q, "QUEUE", sizeof(struct msg_data_related_ *), QUEUE_MEMBER_SIZE, Q_FIFO);

    rt_printf("msgb_producer_init() \n");
    /* Creating cyclic xenomai task */
    ret = rt_task_create(&tcb.producer_task,"producerB_task",0,90,T_FPU);
    if (ret)
    {
        fprintf (stderr, "producerB_task create failed!!!!\n");
        goto release_master;
    }

    /* Starting cyclic task */
    fprintf(stdout, "starting producerB_task \n");
    ret = rt_task_start(&tcb.producer_task, &msgb_producer_task_proc, NULL);
    if (ret)
    {
        fprintf (stderr, "producerB_task start failed!!!!\n");
        goto release_master;
    }
    return &tcb;
release_master:
    return NULL;
}

void msgb_producer_task_proc(void *arg)
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

        msgBData.torque=1.1*messg_index;
        msgBData.current=2.3*messg_index;
        strcpy(msgBData.info," MSG FROM PRODUCER B! \n");

        msgBData.msg_size=sizeof(msgBData.info)+msgBDataWithoutCharValiableSie+sizeof(int);

        msgDataInterface.msgType=MSGBTYPE;

        msgDataInterface.tmsgB=msgBData;

        struct msg_data_related_ *ptr = &msgDataInterface;
        int ret =enqueue(ptr,100);
        if(ret >0)
        {
            std::cout<<"b thread enqueue success"<<std::endl;
        }
        else
        {
             std::cout<<"b thread enqueue failed"<<std::endl;
        }
        std::cout<<"queue size is:"<<get_msg_queue_size()<<std::endl;

        messg_index++;
    }

    //rt_queue_delete(&tcb.q);
}
