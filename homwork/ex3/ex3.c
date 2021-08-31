#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct _foo_struct
{
    int size;
    int *array;
} ARR;
void *foo(ARR *Array)
{

    printf("size:%d\n", Array->size);
    int min = Array->array[0];
    for (int i = 1; i < Array->size; i++)
    {
        if (Array->array[i] < min)
        {
            min = Array->array[i];
        }
    }

    return (void *)min;
}

int main(int argc, char const *argv[])
{
  
   int num = 0;
    for(int i=1;argv[i]!='\0';i++){
      num++;
    }
   // num--;

    ARR array1, array2;
    pthread_t tid1, tid2;
    void *ret;
    if ((num % 2) == 1)
    {

        array1.array = (int *)malloc(((num / 2) + 1) * sizeof(int));
        array2.array = (int *)malloc((num / 2) * sizeof(int));
        array1.size = ((num / 2) + 1);
        array2.size = (num / 2);
    }
    else
    {
        array1.array = (int *)malloc((num / 2) * sizeof(int));
        array2.array = (int *)malloc((num / 2) * sizeof(int));
        array1.size = (num / 2);
        array2.size = (num / 2);
    }

   // printf("enter a numbers to array , num of array:%d\n", num);
    //printf("%d\n",array1.size);
    for (int i = 0; i < array1.size; i++)
    {

         array1.array[i]=atoi(argv[i+1]);
    }
  //  printf("%d\n",array2.size);
    for (int i = array1.size,j=0; i < array2.size+array1.size; j++,i++)
    {
         array2.array[j]=atoi(argv[i+1]);
    }

    if (pthread_create(&tid1, NULL, foo, &array1))
        return 1;

    if (pthread_create(&tid2, NULL, foo, &array2))
        return 1;

    if (pthread_join(tid1, &ret))
    {
        fprintf(stderr, "Could not join Foo Thread\n");

        return 1;
    }
    printf("the min value in Array1:%d\n", (int)ret);
    if (pthread_join(tid2, &ret))
    {
        fprintf(stderr, "Could not join with Bar Thread\n");
        return 1;
    }
    printf("the min value in Array2:%d\n", (int)ret);

    free(array1.array);
    free(array2.array);
    return 0;
}