#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

// 간단한 사용자 락 구조체
struct ulock {
  volatile int locked;
};

// 락 초기화
void initulock(struct ulock *lk) {
  lk->locked = 0;
}

// 락 획득
void ulock_acquire(struct ulock *lk) {
  while(__sync_lock_test_and_set(&(lk->locked), 1) != 0)
    ;  // busy-wait
}

// 락 해제
void ulock_release(struct ulock *lk) {
  __sync_lock_release(&(lk->locked));
}

// 전역 락 선언
struct ulock print_lock;

void busy_loop(int ticks) {
  int i = 0;
  while(i++ < ticks * 1000000);  // 일정 시간 대기
}

void print_process_info(struct pstat *ps) {
  ulock_acquire(&print_lock);  // 락 획득
  for (int i = 0; i < NPROC; i++) {
    if (ps->inuse[i]) {
      printf(1, "PID: %d | Priority: %d | State: %d | Ticks[Q3]: %d\n",
             ps->pid[i], ps->priority[i], ps->state[i], ps->ticks[i][3]);
    }
  }
  ulock_release(&print_lock);  // 락 해제
}

int main() {
  struct pstat ps;

  // 사용자 락 초기화
  initulock(&print_lock);

  // Round Robin 테스트
  printf(1, "\n[TEST] Setting scheduler policy to RR (0)\n");
  setSchedPolicy(0);
  fork();
  fork();
  busy_loop(1);  // 일정 시간 대기
  getpinfo(&ps);
  print_process_info(&ps);

  // MLFQ 테스트
  printf(1, "\n[TEST] Setting scheduler policy to MLFQ (1)\n");
  setSchedPolicy(1);
  fork();
  busy_loop(2);  // 일정 시간 대기
  getpinfo(&ps);
  print_process_info(&ps);

  exit();
}
