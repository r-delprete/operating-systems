/**
 * Stampa messaggi di due thread
 */

#include <pthread.h>
#include <stdio.h>

struct ThreadData
{
  char *message;
  pthread_t id;
  int returnValue;
};

void *printMessage(void *args)
{
  struct ThreadData thread = *((struct ThreadData *)args);

  printf("%s\n", thread.message);
}

int main()
{
  struct ThreadData thread1, thread2;

  thread1.message = "Thread 1";
  thread2.message = "Thread 2";

  thread1.returnValue = pthread_create(&thread1.id, NULL, printMessage, (void *)&thread1);
  thread2.returnValue = pthread_create(&thread2.id, NULL, printMessage, (void *)&thread2);

  pthread_join(thread1.id, NULL);
  pthread_join(thread2.id, NULL);

  printf("Thread 1 exit: %d\n", thread1.returnValue);
  printf("Thread 2 exit: %d\n", thread2.returnValue);

  return 0;
}