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
    int num_thread;

} food_stick;
void *philosophers_problemm(void *f)
{
    // printf("thread_id:%ld\n", pthread_self());
    int tmp = 0, randomTime = 0;
    food_stick *ff = ((food_stick *)f);
    int right_lower = 0, left_higher = 0, back = 0,flag=1;
    if (ff->num_thread + 1 == ff->num)
    {
        right_lower = ff->num_thread;
        left_higher = 0;
        back = right_lower - 1;
    }
    else
    {
        right_lower = ff->num_thread;
        left_higher = ff->num_thread + 1;
        back = right_lower - 1;
    }
    if (right_lower == 0)
    {
        back = ff->num-1;
    }


    while (1)
    {
      
         if(flag==1){
         printf("thread_id:%d trying to pick up a stick\n", pthread_self());
            flag=0;
         }
        pthread_mutex_lock(&mutex);
        if (ff->stick[right_lower] == down && ff->stick[back] == down)
        {
            if (ff->philo_stick[0] == down)
            {
                ff->stick[right_lower] = up;
                ff->philo_stick[0] = up;
                printf("thread_id:%d success pick up a first stick\n", pthread_self());
                 flag=1;
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (ff->stick[left_higher] == down && ff->philo_stick[0] == up && ff->philo_stick[1] == down)
        {

            ff->philo_stick[1] = up;
            printf("thread_id:%d success pick up a secound stick\n", pthread_self());
            randomTime = rand() % 10 + 1;
            pthread_mutex_unlock(&mutex);
            printf("threadID:%ld Begins to eat with time:%d\n", pthread_self(), randomTime);
            sleep(randomTime);

            printf("thread_id:%d I'm done eating with time:%d\n", pthread_self(), randomTime);
            ff->philo_stick[1] = down;
            ff->philo_stick[0] = down;
            ff->stick[right_lower] = down;
            ff->stick[left_higher] = down;
            randomTime = rand() % 11 + 10;
            printf("Begins to think with time:%d\n", randomTime);
            sleep(randomTime);
            printf("I finished thinking with time:%d\n", randomTime);
            flag=1;
        }
        else
            pthread_mutex_unlock(&mutex);

         
    }

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
        food[i].num_thread = i;
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