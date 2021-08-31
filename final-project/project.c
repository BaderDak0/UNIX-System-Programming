#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

enum stick_status
{
  down = 0,
  up
};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct food_stick
{
  enum stick_status philo_stick[2];
  enum stick_status *stick;
  int num;
} food_stick;
void *philosophers_problemm(void *f)
{
  // printf("thread_id:%ld\n", pthread_self());
  int tmp = 0, randomTime = 0;
  food_stick *ff = ((food_stick *)f);
  for (int i = 0; i < ff->num; ++i)
  {
    printf("trying to pick up a stick\n");
    pthread_mutex_lock(&mutex);
    if (ff->stick[i] == down)
    {
      ff->stick[i] = up;
      if (ff->philo_stick[0] == down)
      {
        ff->philo_stick[0] = up;
        printf("success pick up a first stick\n");
        tmp = i;
        pthread_mutex_unlock(&mutex);
      }
      else if (ff->philo_stick[1] == down)
      {
        ff->philo_stick[1] = up;
        printf("success pick up a secound stick\n");
        randomTime = rand() % 10 + 1;
        pthread_mutex_unlock(&mutex);
        printf("threadID:%ld Begins to eat with time:%d\n", pthread_self(), randomTime);
        sleep(randomTime);

        printf("I'm done eating with time:%d\n", randomTime);
        ff->philo_stick[1] = down;
        ff->philo_stick[0] = down;
        ff->stick[i] = down;
        ff->stick[tmp] = down;
        randomTime = rand() % 11 + 10;
        printf("Begins to think with time:%d\n", randomTime);
        sleep(randomTime);
        printf("I finished thinking with time:%d\n", randomTime);
        i = 0;
        // pthread_mutex_unlock(&mutex);
      }
      else
        pthread_mutex_unlock(&mutex);
    }
    else
      pthread_mutex_unlock(&mutex);

    //  printf("thread_id:%d helo %d and %d\n",pthread_self(),ff->stick[i],i);
  }
  // pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, const char *argv[])
{
  int num = atoi(argv[1]);
  pthread_t *tid = (pthread_t *)malloc(num * sizeof(pthread_t));
  enum stick_status *stick = (enum stick_status *)calloc(num, sizeof(enum stick_status));
  food_stick *food = (food_stick *)calloc(num, sizeof(food_stick));
  for (int i = 0; i < num; i++)
  {
    food[i].stick = stick;
    food[i].philo_stick[0] = down;
    food[i].philo_stick[1] = down;
    food[i].num = num;
  }
  for (int i = 0; i < num; i++)
  {
    if (pthread_create(&tid[i], NULL, philosophers_problemm, &food[i]))
      return 1;
  }
  for (int i = 0; i < num; i++)
  {
    pthread_join(tid[i], NULL);
  }
  for (int i = 0; i < num; i++)
  {
    pthread_exit(&tid[i]);
  }
  return 0;
}