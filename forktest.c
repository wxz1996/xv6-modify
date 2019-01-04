// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N  1000



void runtime(int n, int id){
  int maxtime = 100000;
  long t = 0;
  while(t<n)
  {
    t = t+1;
    if(t>maxtime){
	printf(1,"execution time-out!\n");
	kill(id);
    }	
  }
}

void
printf(int fd, const char *s, ...)
{
  write(fd, s, strlen(s));
}

void
forktest(void)
{
  int n;
  
  printf(1, "fork test\n");
  int p1,p2,p3;
  n=N;
  p1=fork();
  if(p1<0) exit();
  if(p1==0){
    printf(1,"p1 running!\n");
    sleep(100);
    ps();
    runtime(10000,4);
    printf(1,"p1 exit!\n");
    exit();
}
  p2=fork();
  if(p2<0) exit();
  if(p2==0){
    printf(1,"p2 running!\n");
    sleep(100);
    ps();
    runtime(100001,5);
    printf(1,"p2 exit!\n");
    exit();
}
  p3=fork();
  if(p3<0) exit();
  if(p3==0){
    printf(1,"p3 running!\n");
    sleep(100);
    ps();
    runtime(10000,6);
    printf(1,"p3 exit!\n");
    exit();
}
  sleep(10);
  ps();

  if(n == N){
    printf(1, "fork claimed to work N times!\n", N);
    exit();
  }

  for(; n > 0; n--){
    if(wait() < 0){
      printf(1, "wait stopped early\n");
      exit();
    }
  }

  if(wait() != -1){
    printf(1, "wait got too many\n");
    exit();
  }

  printf(1, "fork test OK\n");
}

int
main(void)
{
  forktest();
  exit();
}
