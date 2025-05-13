#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>


//////////////////////////////////////////

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//////////////////////////////////////////

int partition(int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void sequential_quicksort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        sequential_quicksort(arr, low, pi - 1);
        sequential_quicksort(arr, pi + 1, high);
    }
}

void parallel_quicksort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);

// #pragma omp task shared(arr) untied
#pragma omp task shared(arr) 
        parallel_quicksort(arr, low, pivot - 1);

// #pragma omp task shared(arr) untied
#pragma omp task shared(arr) 
        parallel_quicksort(arr, pivot + 1, high);
    }
}

void parallel_quicksort_cutoff(int *arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        
        const int cutoff = 10000;
        
        // Левая часть
        #pragma omp task final((high-low) < cutoff) untied 
        {
            if ((high-low) < cutoff) 
                sequential_quicksort(arr, low, pivot - 1);
             else 
                parallel_quicksort_cutoff(arr, low, pivot - 1);
        }
        
        // Создаем задачу для правой части
        #pragma omp task final((high-low) < cutoff) untied 
        {
            if ((high-low) < cutoff)
                sequential_quicksort(arr, pivot + 1, high);
            else 
                parallel_quicksort_cutoff(arr, pivot + 1, high);
        }
        
        // #pragma omp taskwait
    }
}

//////////////////////////////////////////

int *generate_full_unsorted_array(int size)
{
    int *arr = (int *)malloc(size * sizeof(int));
    for (size_t i = 0; i < size; i++)
        arr[i] = size - i;

    return arr;
}

int *generate_half_unsorted_array(int size)
{
    int *arr = (int *)malloc(size * sizeof(int));
    for (size_t i = 0; i < size; i++)
        arr[i] = i < size / 2 ? i : size - i;

    return arr;
}

int *generate_full_sorted_array(int size)
{
    int *arr = (int *)malloc(size * sizeof(int));
    for (size_t i = 0; i < size; i++)
        arr[i] = i;

    return arr;
}

int is_sorted(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            return 0;
        }
    }
    return 1;
}

//////////////////////////////////////////

void merge_two_arrays(int *arr1, int n1, int *arr2, int n2)
{
    int *temp = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2)
    {
        if (arr1[i] < arr2[j])
        {
            temp[k++] = arr1[i++];
        }
        else
        {
            temp[k++] = arr2[j++];
        }
    }

    while (i < n1)
        temp[k++] = arr1[i++];
    while (j < n2)
        temp[k++] = arr2[j++];

    for (i = 0; i < n1 + n2; i++)
    {
        arr1[i] = temp[i];
    }

    free(temp);
}

void merge_all_parts(
    int *arr,
    int part_size,
    int num_parts,
    int total_size)
{
    // Обрабатка остатка, если общий размер не делится нацело
    int remainder = total_size % num_parts;
    if (remainder != 0)
    {
        int *remainder_data = arr + total_size - remainder;
        sequential_quicksort(
            remainder_data,
            0,
            remainder - 1);
        merge_two_arrays(
            arr,
            total_size - remainder,
            remainder_data,
            remainder);
    }

    // Сливание отсортированных частей
    for (int step = 1; step < num_parts; step *= 2)
    {
        for (int i = 0; i < num_parts; i += 2 * step)
        {
            if (i + step < num_parts)
            {
                int start = i * part_size;
                int mid = (i + step) * part_size;
                int end = (i + 2 * step) * part_size;
                if (end > total_size)
                    end = total_size;

                merge_two_arrays(
                    arr + start,
                    mid - start,
                    arr + mid,
                    end - mid);
            }
        }
    }
}

//////////////////////////////////////////

int main(int argc, char **argv)
{
    char mode = 'u';
    int rank, amount_proc;
    int *data = NULL;
    int *local_data = NULL;
    int n = 1000000; // Размер массива по умолчанию
    int local_n;
    double total_start_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &amount_proc);

    if (argc > 2)
    {
        n = atoi(argv[1]);
        mode = *(argv[2]);
    }

    local_n = n / amount_proc;
    // int remainder = n % amount_proc;
    // if (rank < remainder) {
    //     local_n++;
    // }

    // Выделение памяти и генерация данных в процессе 0
    if (rank == 0)
    {
        switch (mode)
        {
        case 'u':
        case 'U':
            data = generate_full_unsorted_array(n);
            break;
        case 's':
        case 'S':
            data = generate_full_sorted_array(n);
            break;
        case 'h':
        case 'H':
            data = generate_half_unsorted_array(n);
            break;

        default:
            printf("mode \'%c\' not defined\n", mode);
            exit(0);
        }

        printf("Generated array of size %d with mode \'%c\'\n", n, mode);

        total_start_time = MPI_Wtime();
    }

    // Распределение данных
    local_data = (int *)malloc(local_n * sizeof(int));
    MPI_Scatter(
        data, local_n, MPI_INT,
        local_data, local_n, MPI_INT,
        0, MPI_COMM_WORLD);

    printf("Process %d received %d elements\n", rank, local_n);

    // Параллельная сортировка
    double start_time = MPI_Wtime();

#pragma omp parallel 
    {
#pragma omp single
        {
            #pragma omp task untied
            parallel_quicksort(local_data, 0, local_n - 1);
            // parallel_quicksort_cutoff(local_data, 0, local_n - 1);
        }
    }

    double sort_time = MPI_Wtime() - start_time;
    printf("Process %d sorted its part in %.4f seconds\n", rank, sort_time);

    // Сбор результатов
    MPI_Gather(local_data, local_n, MPI_INT,
               data, local_n, MPI_INT,
               0, MPI_COMM_WORLD);

    // Финальное слияние и проверка в процессе 0
    if (rank == 0)
    {
        start_time = MPI_Wtime();
        merge_all_parts(data, local_n, amount_proc, n);
        double merge_time = MPI_Wtime() - start_time;

        printf("Merge completed in %.4f seconds\n", merge_time);

        double total_time = MPI_Wtime() - total_start_time;

        printf("Procedure completed in %.4f seconds\n", total_time);

        if (is_sorted(data, n))
            printf("Array is correctly sorted!\n");
        else
            printf("Error: Array is not sorted correctly!\n");

        free(data);
    }

    free(local_data);
    MPI_Finalize();

    return 0;
}