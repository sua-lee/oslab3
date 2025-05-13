#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

#define BUF_SIZE 1024

// 함수 선언
void busy_loop(int ticks);
void print_process_info(struct pstat *ps);
void wait_for_children();

// 바쁜 대기 루프 함수
void busy_loop(int ticks) {
  int i = 0;
  while(i++ < ticks * 1000000);  // 일정 시간 대기
}

// 프로세스 정보를 보기 좋게 출력
void print_process_info(struct pstat *ps) {
  printf(1, "\n---------------------------------\n");
  printf(1, "PID\tPriority\tState\tTicks[Q3]\n");
  printf(1, "---------------------------------\n");
  for (int i = 0; i < NPROC; i++) {
    if (ps->inuse[i]) {
      printf(1, "%d\t%d\t\t%d\t%d\n", ps->pid[i], ps->priority[i], ps->state[i], ps->ticks[i][3]);
    }
  }
  printf(1, "---------------------------------\n");
}

// 자식 프로세스 대기
void wait_for_children() {
  int wpid;
  while((wpid = wait()) > 0) {
    printf(1, "Child process %d exited.\n", wpid);
  }
}

// 테스트 로그를 보기 좋게 출력
void print_test_log(const char *message) {
  printf(1, "\n[TEST] %s\n", message);
}

int main() {
  struct pstat ps;
  int pid1, pid2;

  // Round Robin 테스트
  print_test_log("Setting scheduler policy to RR (0)");
  setSchedPolicy(0);

  // 자식 프로세스 생성
  if ((pid1 = fork()) == 0) {
    busy_loop(1);
    exit();
  }

  if ((pid2 = fork()) == 0) {
    busy_loop(1);
    exit();
  }

  // 부모 프로세스: 자식 종료 대기
  wait_for_children();

  getpinfo(&ps);
  print_process_info(&ps);

  // MLFQ 테스트
  print_test_log("Setting scheduler policy to MLFQ (1)");
  setSchedPolicy(1);

  // 자식 프로세스 생성
  if ((pid1 = fork()) == 0) {
    busy_loop(2);
    exit();
  }

  if ((pid2 = fork()) == 0) {
    busy_loop(2);
    exit();
  }

  // 부모 프로세스: 자식 종료 대기
  wait_for_children();

  getpinfo(&ps);
  print_process_info(&ps);

  exit();
}
