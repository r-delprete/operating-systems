#include "../libs/apue.h"
#include <pthread.h>

pthread_t threadID;

void printThreadID(const char *msg)
{
  pid_t processID = getpid();
  pthread_t tID = pthread_self();

  printf("%s\t process ID = %lu\t thread ID = %lu\t (0x&lx)\n", msg, processID, (unsigned long)tID, (unsigned long)tID);
}

void *threadRoutine(void *args)
{
  printThreadID("New thread: ");

  return ((void *)0);
}

int main()
{
  int err;

  if ((err = pthread_create(&threadID, NULL, threadRoutine, NULL)) != 0)
  {
    fprintf(stderr, "Error creating thread: %s", strerror(err));
    return -1;
  }

  printThreadID("Main thread: \t");
  sleep(1);

  return 0;
}
