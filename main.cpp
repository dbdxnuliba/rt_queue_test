#include <iostream>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <rtdm/rtdm.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/event.h>
#include <native/mutex.h>
#include <native/timer.h>
#include<native/queue.h>
#include <rtdk.h>
using namespace std;
#include "msga_producer_task.h"
#include "msgb_producer_task.h"
#include "msgc_producer_task.h"
#include "consumer_task.h"

typedef struct
{
    void *proTcb;
    void *proCon;

}relase_t;

static  relase_t trelase_t;
void signal_handler_in_main(int)
{
    //     consumer_deinit(arg->proCon);
    //producer_deinit(arg->proTcb);
}

int main()
{
    int ret=0;
    queue_init();
    void *proTcbB = msgb_producer_init();
    void *proTcbA = msga_producer_init();
    msgc_producer_init();

    void *proCon = consumer_init();

    //    trelase_t.proCon=proTcb;
    //    trelase_t.proTcb=proCon;


    //    signal(SIGTERM, signal_handler_in_main);//
    //    signal(SIGINT, signal_handler_in_main);//CTRL+C触发该函数
    while(1)
    {
        usleep(1000);
        //sched_yield();
    }
    return 0;

release_master:
    fprintf(stdout, "End of Program\n");
    return 0;
}

