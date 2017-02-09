//
//  opencl.cpp
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

#include "opencl.h"


int InitOpenCL()
{
    int err;
    int use_gpu = 0;
    
    err = clGetDeviceIDs(NULL, use_gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
    
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (!command_queue)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
    return 0;
}

int executeKernel(const char* kernelFilePath, const char* kernelName, const float* in, float* out, int count)
{
    int err;
    // Load Kernel from file
    FILE* file = fopen(kernelFilePath, "rb");
    if (!file)
    {
        printf("Error: Failed to load kernel from file %s!\n", kernelFilePath);
        return EXIT_FAILURE;
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *kernelSourceCode = (char*)malloc(fsize + 1);
    fread(kernelSourceCode, fsize, 1, file);
    fclose(file);
    
    kernelSourceCode[fsize] = 0;
    //
    
    program = clCreateProgramWithSource(context, 1, const_cast<const char**>(&kernelSourceCode), NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }
    
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }
    
    kernel = clCreateKernel(program, kernelName, &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        return EXIT_FAILURE;
    }
    
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL);
    if (!in || !out)
    {
        printf("Error: Failed to allocate device memory!\n");
        return EXIT_FAILURE;
    }
    
    err = clEnqueueWriteBuffer(command_queue, input, CL_TRUE, 0, sizeof(float) * count, in, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        return EXIT_FAILURE;
    }
    
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        return EXIT_FAILURE;
    }
    
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local_domain_size), &local_domain_size, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        return EXIT_FAILURE;
    }
    
    global_domain_size = count;
    err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_domain_size, &local_domain_size, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }
    
    // Wait for commands to finish
    clFinish(command_queue);
    
    err = clEnqueueReadBuffer( command_queue, output, CL_TRUE, 0, sizeof(float) * count, out, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        return EXIT_FAILURE;
    }
    
    return 0;
}

int CleanUpOpenCL()
{
    int err;
    
    err = clReleaseMemObject(input);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release memory object! %d\n", err);
        return EXIT_FAILURE;
    }
    err = clReleaseMemObject(output);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release memory object! %d\n", err);
        return EXIT_FAILURE;
    }
    err = clReleaseProgram(program);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release program! %d\n", err);
        return EXIT_FAILURE;
    }
    err = clReleaseKernel(kernel);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release kernel! %d\n", err);
        return EXIT_FAILURE;
    }
    err = clReleaseCommandQueue(command_queue);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release command queue! %d\n", err);
        return EXIT_FAILURE;
    }
    err = clReleaseContext(context);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to release context! %d\n", err);
        return EXIT_FAILURE;
    }
    
    return 0;
}

int TestOpenCL()
{
    int correct = 0;
    int count = 1024*8;
    float data[count];
    float *dataStart = data;
    float results[count];
    
    int i = 0;
    for(i = 0; i < count; ++i)
    {
        data[i] = i;
    }
    
    InitOpenCL();
    executeKernel("src/helloWorld.cl", "helloWorld", const_cast<const float*>(dataStart), results, count);
    CleanUpOpenCL();
    
    for(i = 0; i < count; i++)
    {
        if(results[i] == data[i] * data[i])
        {
            correct++;
        }
    }
    
    printf("Computed '%d/%d' correct values!\n", correct, count);
    
    
    return 0;
}
