//
//  opencl.h
//  InstagramEffects
//
//  Created by Viktor Ketipov
//

#ifndef InstagramEffects_opencl_h
#define InstagramEffects_opencl_h

#ifdef __APPLE__
#   include "OpenCL/cl.h"
#else
#   include "cl.h"
#endif

static size_t global_domain_size;
static size_t local_domain_size;

static cl_device_id device_id;
static cl_context context;
static cl_command_queue command_queue;
static cl_program program;
static cl_kernel kernel;

static cl_mem clInputBuffer;
static cl_mem clOutputBuffer;
static cl_mem clFilterBuffer;
static cl_mem clFilterSize;

typedef struct
{
    float* data;
    int size;
}convolutionFilter;

int InitOpenCL();

//int executeKernel(const char* kernelFilePath, const char* kernelName, const float* in, float* out, int count)

int CleanUpOpenCL();

int HelloWorldOpenCL();

//int executeImageKernelKernel(size_t width, size_t height, void* inputPixels, void* outputPixels);

int RunOpenCLKernel(const char* kernelFilepath, const char* kernalName, int width, int height, void* inputPixels, void* outputPixels);

int RunConvolutionOpenCLKernel(int width, int height, void* inputPixels, void* outputPixels, convolutionFilter filter);

#endif
