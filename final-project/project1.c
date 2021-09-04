#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<unistd.h>
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

    int tmp = 0, randomTime = 0;
    food_stick *ff = ((food_stick *)f);
    int right_lower = 0, left_higher = 0, back = 0, flag = 1;
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
        back = ff->num - 1;
    }

    while (1)
    {

        if (flag == 1)
        {
            printf("philosoph:%d trying to pick up a stick\n", (ff->num_thread + 1));
            flag = 0;
        }
        pthread_mutex_lock(&mutex);
        if (ff->stick[right_lower] == down && ff->stick[back] == down) // check if neighbor is not in need the secound stick 
        {
            if (ff->philo_stick[0] == down)
            {
                ff->stick[right_lower] = up;
                ff->philo_stick[0] = up;
                printf("philosoph:%d success pick up a first stick\n", (ff->num_thread + 1));
                flag = 1;
            }
            pthread_mutex_unlock(&mutex);
        }
        else if (ff->stick[left_higher] == down && ff->philo_stick[0] == up && ff->philo_stick[1] == down)
        {

            ff->philo_stick[1] = up;
            printf("philosoph:%d success pick up a secound stick\n", (ff->num_thread + 1));
            randomTime = rand() % 10 + 1;
            pthread_mutex_unlock(&mutex);
            printf("philosoph:%ld Begins to eat with time:%d\n", (ff->num_thread + 1), randomTime);
            sleep(randomTime);

            printf("philosoph:%d I'm done eating with time:%d\n", (ff->num_thread + 1), randomTime);

            ff->philo_stick[1] = down;
            ff->philo_stick[0] = down;
              pthread_mutex_lock(&mutex);
            ff->stick[right_lower] = down;
            ff->stick[left_higher] = down;
             pthread_mutex_unlock(&mutex);
            randomTime = rand() % 11 + 10;
            printf("philosoph:%d Begins to think with time:%d\n", (ff->num_thread + 1), randomTime);
            sleep(randomTime);
            printf("philosoph:%d I finished thinking with time:%d\n", (ff->num_thread + 1), randomTime);
            flag = 1;
        }
        else
            pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, const char *argv[])
{
    int num = atoi(argv[1]);
    if (num <= 1)
    {
        printf("cannot do a feast with this number\n");
        return 0;
    }

    pthread_t *tid = (pthread_t *)malloc(num * sizeof(pthread_t));
    if (tid == NULL)
    {
        printf("Error ALLOCATION Memory\n");
        return 0;
    }
    enum stick_status *stick = (enum stick_status *)calloc(num, sizeof(enum stick_status));
    if (stick == NULL)
    {
        printf("Error ALLOCATION Memory\n");
        return 0;
    }
    food_stick *food = (food_stick *)calloc(num, sizeof(food_stick));
    if (food == NULL)
    {
        printf("Error ALLOCATION Memory\n");
        return 0;
    }

    for (int i = 0; i < num; i++)
    {
        food[i].stick = stick;
        food[i].philo_stick[0] = down;
        food[i].philo_stick[1] = down;
        food[i].num = num;
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
    free(tid);
    free(stick);
    free(food);
    return 0;
}