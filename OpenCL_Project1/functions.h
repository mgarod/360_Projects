#ifndef __HelloWorld__functions__
#define __HelloWorld__functions__
#include <OpenCL/opencl.h>
#define ARRAY_SIZE 1000

cl_context CreateContext();

cl_command_queue CreateCommandQueue(cl_context, cl_device_id *);

cl_program CreateProgram(cl_context, cl_device_id, const char *);

bool CreateMemObjects(cl_context, cl_mem memObjects[3], float *, float *);

void Cleanup(cl_context context, cl_command_queue commandQueue,
             cl_program program, cl_kernel kernel, cl_mem memObjects[3]);

#endif /* defined(__HelloWorld__functions__) */