#include "param.h"

struct pstat {
  // whether this slot of the process table is in use (1 or 0)
  int inuse[NPROC]; 
  // PID of each process
  int pid[NPROC];   
  // current priority level of each process (0-3)
  int priority[NPROC];  
  // current state (e.g., SLEEPING or RUNNABLE) of each process
  // see enum procstate
  int state[NPROC];  
  // number of ticks each process has accumulated 
  // RUNNING/SCHEDULED at each of 4 priorities
  int ticks[NPROC][4];  
  // number of ticks each process has waited before being scheduled
  int wait_ticks[NPROC][4]; 
};