#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

#define NQUEUE 4  // MLFQ의 우선순위 큐 개수

struct pstat {
    int inuse[NPROC];       // 사용 여부
    int pid[NPROC];         // 프로세스 ID
    int priority[NPROC];    // 우선순위 (0-3)
    int state[NPROC];       // 프로세스 상태 (RUNNING 등)
    int ticks[NPROC][NQUEUE];      // 각 우선순위 큐에서 사용한 틱 수
    int wait_ticks[NPROC][NQUEUE]; // 각 큐에서 대기한 시간
};

#endif  // _PSTAT_H_