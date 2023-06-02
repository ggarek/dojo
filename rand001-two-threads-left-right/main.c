#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// create shared mutex
// create shared signal
// create shared step variable
// write step function
// start two threads with L and R

static void* thread_step(void *arg);
void die(int notzero, char* message);

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static current_step = 1; // 1 is Left, 2 is Right

int main() {
  printf("hello\n");

  pthread_t t1;
  pthread_t t2;
  int s;

  s = pthread_create(&t1, NULL, thread_step, (void*)1);
  die(s, "pthread_create");
 
  s = pthread_create(&t2, NULL, thread_step, (void*)2);
  die(s, "pthread_create");


  s = pthread_join(t1, NULL);
  die(s, "pthread_join"); 

  s = pthread_join(t2, NULL);
  die(s, "pthread_join"); 
}

static void* thread_step(void *arg) {
  int x = (int) arg;
  int s = 0;
  int steps = 10;
  
  while(steps--) {
    s = pthread_mutex_lock(&mx);
    die(s, "pthread_mutex_lock");

    while (current_step != x) {
      s = pthread_cond_wait(&cond, &mx);
      die(s, "pthread_cond_wait");
    }

    printf(x == 1 ? "L" : "R");
    current_step = x == 1 ? 2 : 1;
    s = pthread_cond_signal(&cond);
    die(s, "pthread_cond_signal");
    
    s = pthread_mutex_unlock(&mx);
    die(s, "pthread_mutex_unlock");
  }

  return NULL;
}

void die(int notzero, char* message) {
  if (notzero == 0) return;
  printf("%s\n", message);
  exit(notzero);
}
