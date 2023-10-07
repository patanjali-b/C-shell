1. 
Added :
  int is_sigalarm;
  int ticks;
  int now_ticks;
  uint64 handler;
  struct trapframe *trapframe_copy;  // cache the trapframe when timer fires (basically to store the state of the process)
  

to the proc.h file in the struct proc definition.

2.
Added : 
if ((p->trapframe_copy = (struct trapframe *)kalloc()) == 0)
  {
    release(&p->lock);
    return 0;
  }
  p->is_sigalarm = 0;
  p->ticks = 0;
  p->now_ticks = 0;
  p->handler = 0;

to the allocproc() function in proc.c

3.



