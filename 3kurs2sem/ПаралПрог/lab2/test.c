#include <omp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t calc(size_t i)
{
    double dividend = sin(i) + cos(2 * i) + pow(i, 3);
    double divisor = sqrt(i + 1) + log(i + 1);
    return (uint64_t)(dividend / divisor);
}

uint64_t func(size_t num_iter, char key)
{
    uint64_t sum = 0;
    if (key == 's')
    {
        #pragma omp parallel for schedule(static) reduction(+ : sum)
        for (size_t i = 0; i < num_iter; i++)
        {
            sum += calc(i);
        }
    }
    else if (key == 'd')
    {
        #pragma omp parallel for schedule(dynamic) reduction(+ : sum)
        for (size_t i = 0; i < num_iter; i++)
        {
            sum += calc(i);
        }
    }
    else if (key == 'g')
    {
        #pragma omp parallel for schedule(guided) reduction(+ : sum)
        for (size_t i = 0; i < num_iter; i++)
        {
            sum += calc(i);
        }
    }
    else if (key == 'a')
    {
        #pragma omp parallel for schedule(auto) reduction(+ : sum)
        for (size_t i = 0; i < num_iter; i++)
        {
            sum += calc(i);
        }
    }
    else
    {
        printf("Unknown schedule type: %c\n", key);
    }

    return sum;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
        return 1;

    size_t num_iter = atoll(argv[1]);
    char key = *argv[2];

    func(num_iter, key);

    return 0;
}