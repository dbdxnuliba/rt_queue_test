#ifndef MSG_QUEUE_MANAGER_H
#define MSG_QUEUE_MANAGER_H
#include <native/queue.h>
#include <memory.h>
#include <native/event.h>
#include <native/mutex.h>
#include <native/timer.h>
#include <native/task.h>
#include <rtdk.h>
#include <queue>

#define SM_FRAME_PERIOD_NS      2*100000000
#define QUEUE_MEMBER_SIZE   20
#define msgAValiableSize  2*sizeof(int)
#define msgBMaxValiableSize  128*sizeof(char)+sizeof(float)
#define msgBDataWithoutCharValiableSie   2*sizeof(float)



enum enuMsgType{MSGATYPE=1,MSGBTYPE,MSGCTYPE};
typedef struct {
    RT_TASK producer_task;
}pro_info_t;


//typedef struct {
//    RT_QUEUE q;
//}queue_info_t;

struct msgA_data_related_{
    int vel;
    int acc;
    int msg_size;

};

struct msgB_data_related_{
    char info[128];
    float torque;
    float current;
    int msg_size;
};

struct msgC_data_related_{
    char info[128];
};


struct msg_data_related_{
    enuMsgType msgType;
    union
    {
        struct msgA_data_related_ tmsgA;
        struct msgB_data_related_ tmsgB;
        struct msgC_data_related_ tmsgC;
    } ;

};

void  queue_init();
int enqueue(msg_data_related_ *ptr,uint32_t wait_usecond);
int dequeue(msg_data_related_ &tmsg_data_related_,uint32_t wait_usecond);
int get_msg_queue_size(void);
#endif // MSG_QUEUE_MANAGER_H
