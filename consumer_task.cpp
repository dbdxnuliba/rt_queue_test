#include "consumer_task.h"
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
#include <stdlib.h>
#include "msga_producer_task.h"

typedef struct {
    int       exit;

    RT_TASK   consumer_task;
}con_info_t;

void consumer_task_proc(void *arg);

void *consumer_init()
{
    int ret = 0;
    con_info_t *tcb = (con_info_t *)calloc(1,sizeof(con_info_t));//calloc在动态分配完内存后，自动初始化该内存空间为零，而malloc不初始化，里边数据是随机的垃圾数据。

    //tcb->pRcvQ = (RT_QUEUE*)get_producer_queue(arg);
    rt_printf("step1 \n");
    /* Creating cyclic xenomai task */
    ret = rt_task_create(&tcb->consumer_task,"consumer_task",0,70,T_FPU);
    if (ret)
    {
        fprintf (stderr, "producer_task create failed!!!!\n");
        goto release_master;
    }

    /* Starting cyclic task */
    fprintf(stdout, "starting consumer_task \n");
    ret = rt_task_start(&tcb->consumer_task, &consumer_task_proc, tcb);
    if (ret)
    {
        fprintf (stderr, "consumer_task start failed!!!!\n");
        goto release_master;
    }
    return tcb;
release_master:
    return NULL;
}

void consumer_deinit(void *arg)
{
    con_info_t *tcb = (con_info_t *)arg;
    if(tcb == NULL) return ;
    tcb->exit = 1;
    while(tcb->exit == 1)
    {
        usleep(1000);
    }
    rt_task_join(&tcb->consumer_task);
    rt_task_delete(&tcb->consumer_task);
    free(tcb);
}

void consumer_task_proc(void *arg)
{
    int count=0;
    con_info_t *tcb = (con_info_t *)arg;

    tcb->exit = 0;

    /* Set Xenomai task execution mode */
    int ret;
    ret = rt_task_set_mode(0, T_CONFORMING, NULL);
    if (ret)
    {
        rt_printf("error while rt_task_set_mode, code %d\n",ret);
        return;
    }

    rt_task_set_periodic(NULL,TM_NOW,SM_FRAME_PERIOD_NS);
    /* Start pdo exchange loop until user stop */
    while (tcb->exit == 0)
    {
        if(rt_task_wait_period(NULL)!=0)
        {
            rt_printf("sched_rms_period error_in_consumer_task_proc!\n");

        }
        struct msg_data_related_ rmsg;

        ret =dequeue(rmsg,100);

//        ret = rt_queue_read(tcb->pRcvQ, &msg, sizeof(struct msg_data_related_ *), NULL);
        if(ret>0)
        {

            switch (rmsg.msgType)
            {
            case  MSGATYPE:
            {
                printf("consumer get msg->vel is:%d\n",rmsg.tmsgA.vel);
                printf("consumer get msg->acc is:%d\n",rmsg.tmsgA.acc);
                break;
            }
            case MSGBTYPE:
            {
                printf("consumer get msg info is:%s\n",rmsg.tmsgB.info);
                break;

            }
            case MSGCTYPE:
            {
                printf("consumer get msg info is:%s\n",rmsg.tmsgC.info);
                break;

            }

            }



        }
        if(count%100==0)
        {
            rt_printf("consumer_task_proc exectuting \n");

        }
        count++;
    }
    tcb->exit = 0;
}

