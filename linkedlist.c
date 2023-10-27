#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node
{
    int data;
    int fibdata;
    struct node *next;
};

int fib(int n)
{
    int x, y;
    if (n < 2)
    {
        return (n);
    }
    else
    {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node *p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node *init_list(struct node *p)
{
    int i;
    struct node *head = NULL;
    struct node *temp = NULL;

    head = malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++)
    {
        temp = malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[])
{
    int num_threads = atoi(argv[1]);

    int counter = 0;
    double start, end;
    double start2, end2;
    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    while (p != NULL)
    {
        p = p->next;
        counter++;
    }

    struct node *pointers[counter];

    for (int i = 0; i < counter; i++)
    {
        pointers[i] = head;
        head = head->next;
    }

    start = omp_get_wtime();
    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
#pragma omp master
        printf("threads: %d\n", omp_get_num_threads());
#pragma omp for
        for (int i = 0; i < counter; i += num_threads)
        {
            processwork(pointers[i]);
            printf("%d : %d\n", pointers[i]->data, pointers[i]->fibdata);
        }
    }

    end = omp_get_wtime();

    p = head;
    while (p != NULL)
    {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}
