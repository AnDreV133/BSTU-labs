#define CL_TARGET_OPENCL_VERSION 200

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <chrono>
#include <iostream>

#define N 134217728
#define EPSILON 0.01

using namespace std;

typedef struct
{
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_device_id device;
} OpenCLContext;

int check_error(cl_int err, const char *msg)
{
    return (err != CL_SUCCESS)
               ? (fprintf(stderr, "%s failed: %d\n", msg, err), 0)
               : 1;
}

int setup_opencl(OpenCLContext *cl_ctx)
{
    cl_platform_id platform;
    cl_int err;
    int ok = 1;
    err = clGetPlatformIDs(1, &platform, NULL);
    ok &= check_error(err, "clGetPlatformIDs");

    if (ok)
    {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &cl_ctx->device, NULL);
        ok &= check_error(err, "clGetDeviceIDs");
    }
    if (ok)
    {
        cl_ctx->context = clCreateContext(NULL, 1, &cl_ctx->device, NULL, NULL, &err);
        ok &= check_error(err, "clCreateContext");
    }
    if (ok)
    {
        cl_command_queue_properties props[] = {CL_QUEUE_PROPERTIES,
                                               CL_QUEUE_PROFILING_ENABLE,
                                               0};

        cl_ctx->queue = clCreateCommandQueueWithProperties(cl_ctx->context,
                                                           cl_ctx->device,
                                                           props,
                                                           &err);
        ok &= check_error(err, "clCreateCommandQueueWithProperties");
    }
    if (ok)
    {
        const char *kernel_source =
            "__kernel void sum_squares(__global const float* input,\n"
            "                          __global float* partial_sums,\n "
            "                            const unsigned int N,\n"
            "                          __local float* local_data) {\n"
            "    int gid        = get_global_id(0);\n"
            "    int lid        = get_local_id(0);\n"
            "    int group_size = get_local_size(0);\n"
            "    float val      = 0.0f;\n"
            "    if (gid < N) val = sin(input[gid]) * cos(input[gid]);\n"
            "    local_data[lid] = val;\n"
            "    barrier(CLK_LOCAL_MEM_FENCE);\n"
            "    for (int stride = group_size >> 1; stride > 0; stride >>= 1) {\n"
            "        if (lid < stride) \n"
            "            local_data[lid] += local_data[lid + stride];\n"
            "        barrier(CLK_LOCAL_MEM_FENCE);\n"
            "    }\n"
            "    if (!lid) partial_sums[get_group_id(0)] = local_data[0];\n"
            "}";
        cl_ctx->program = clCreateProgramWithSource(cl_ctx->context,
                                                    1,
                                                    &kernel_source,
                                                    NULL,
                                                    &err);

        ok &= check_error(err, "clCreateProgramWithSource");
    }
    if (ok)
    {
        err = clBuildProgram(cl_ctx->program, 1, &cl_ctx->device, NULL, NULL, NULL);
        ok &= check_error(err, "clBuildProgram");
    }
    if (ok)
    {
        cl_ctx->kernel = clCreateKernel(cl_ctx->program, "sum_squares", &err);
        ok &= check_error(err, "clCreateKernel");
    }
    return ok;
}

void cleanup(OpenCLContext *cl_ctx)
{
    clReleaseKernel(cl_ctx->kernel);
    clReleaseProgram(cl_ctx->program);
    clReleaseCommandQueue(cl_ctx->queue);
    clReleaseContext(cl_ctx->context);
}

float calculate_cpu(float *input, const unsigned int size)
{
    float cpu_result = 0.0f;
    for (unsigned int idx = 0; idx < size; ++idx)
        cpu_result += sinf(input[idx]) * cosf(input[idx]);
    return cpu_result;
}

int run_kernel(OpenCLContext *cl_ctx, float *input, float *result_out, const unsigned int size)
{
    const int LOCAL_SIZE = 256;
    int ok = 1;
    size_t global_size = ((size + LOCAL_SIZE - 1) / LOCAL_SIZE) * LOCAL_SIZE;
    size_t local_size = LOCAL_SIZE;
    size_t num_groups = global_size / local_size;
    cl_mem input_buf = clCreateBuffer(cl_ctx->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * size, input, NULL);
    cl_mem partial_buf = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY,
                                        sizeof(float) * num_groups, NULL, NULL);
    ok &= check_error(clSetKernelArg(cl_ctx->kernel, 0, sizeof(cl_mem), &input_buf), "set arg 0");
    ok &= check_error(clSetKernelArg(cl_ctx->kernel, 1, sizeof(cl_mem), &partial_buf), "set arg 1");
    unsigned int n_val = size;
    ok &= check_error(clSetKernelArg(cl_ctx->kernel, 2, sizeof(unsigned int), &n_val), "set arg 2");
    ok &= check_error(clSetKernelArg(cl_ctx->kernel, 3,
                                     LOCAL_SIZE * sizeof(float), NULL),
                      "set arg 3 (local)");
    cl_event event;
    ok &= check_error(clEnqueueNDRangeKernel(cl_ctx->queue, cl_ctx->kernel, 1, NULL,
                                             &global_size, &local_size, 0, NULL, &event),
                      "enqueue kernel");
    clWaitForEvents(1, &event);
    float *partial_sums = (float *)malloc(sizeof(float) * num_groups);
    ok &= check_error(clEnqueueReadBuffer(cl_ctx->queue, partial_buf, CL_TRUE, 0, sizeof(float) * num_groups, partial_sums, 0, NULL, NULL), "read buffer");
    *result_out = 0.0f;
    for (size_t idx = 0; idx < num_groups; ++idx)
        *result_out += partial_sums[idx];
    free(partial_sums);
    clReleaseMemObject(input_buf);
    clReleaseMemObject(partial_buf);
    clReleaseEvent(event);
    return ok;
}

typedef struct
{
    float *input;
    float *result;
    unsigned int size;
} CpuTaskArgs;

void cpu_compute(void *args)
{
    CpuTaskArgs *cpu_args = (CpuTaskArgs *)args;
    *(cpu_args->result) = calculate_cpu(cpu_args->input,
                                        cpu_args->size);
}

typedef struct
{
    OpenCLContext *cl_ctx;
    float *input;
    float *result;
    int *ok;
    unsigned int size;
} GpuTaskArgs;

void gpu_compute(void *args)
{
    GpuTaskArgs *gpu_args = (GpuTaskArgs *)args;
    *(gpu_args->ok) = run_kernel(gpu_args->cl_ctx,
                                 gpu_args->input,
                                 gpu_args->result,
                                 gpu_args->size);
}

double measure_time(void (*func)(void *), void *arg)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    func(arg);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(end - begin).count();
}

int main()
{
    float cpu_result,
        gpu_result;
    double cpu_time,
        gpu_time;
    float *input = (float *)malloc(sizeof(float) * N);
    
    for (unsigned int idx = 0; idx < N; ++idx)
        input[idx] = sinf(idx);
    CpuTaskArgs cpu_args = {
        .input = input,
        .result = &cpu_result,
        .size = N,
    };

    cpu_time = measure_time(cpu_compute, &cpu_args);
    printf("spend CPU: %.0f ms\n", cpu_time);
    printf("result CPU: %.0f\n", cpu_result);
    OpenCLContext cl_ctx;
    int ok = setup_opencl(&cl_ctx);
    if (ok)
    {
        GpuTaskArgs gpu_args = {
            .cl_ctx = &cl_ctx,
            .input = input,
            .result = &gpu_result,
            .ok = &ok,
            .size = N,
        };
        gpu_time = measure_time(gpu_compute, &gpu_args);
    }
    if (ok)
    {
        printf("spend GPU: %.0f ms\n", gpu_time);
        printf("result GPU: %0f\n", gpu_result);
        printf("check: %s\n", fabs(cpu_result - gpu_result) < EPSILON ? "GOOD" : "BAD");
    }
    free(input);
    cleanup(&cl_ctx);
    return ok ? 0 : 1;
}