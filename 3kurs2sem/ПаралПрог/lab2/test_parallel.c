#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define NUM_THREADS 4

struct args
{
    int core_id;
    uint64_t num_iters;
};

uint64_t func(size_t num_iter)
{
    volatile uint64_t sum = 0;
    for (size_t i = 0; i < num_iter; i++)
    {
        double dividend = sin(i) + cos(2 * i) + pow(i, 3);
        double divisor = sqrt(i + 1) + log(i + 1);
        sum += (uint64_t)(dividend / divisor);
    }

    return sum;
}

// Принудительное закрепление потока за конкретным ядром
void pin_thread_to_core(int core_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void *compute_pinned(void *args)
{
    struct args *args_by_struct = (struct args *)args;

    int core_id = args_by_struct->core_id;
    pin_thread_to_core(core_id);

    uint64_t num_iters = args_by_struct->num_iters;
    func(num_iters);
}

int main(int argc, char const *argv[])
{
    uint64_t num_iters = atoll(argv[1]);
    printf("%lu\n", num_iters);

    pthread_t threads[NUM_THREADS];
    for (size_t iter = 0; iter < NUM_THREADS; iter++)
    {
        struct args args = {iter, num_iters};
        pthread_create(&threads[iter], NULL, compute_pinned, (void *)&args);
    }

    for (size_t iter = 0; iter < NUM_THREADS; iter++)
        pthread_join(threads[iter], NULL);

    return 0;
}