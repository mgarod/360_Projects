#include <OpenCL/opencl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "functions.h"
using namespace std;

cl_context CreateContext()
{
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformID;
    cl_context context=NULL;

    //Select an OpenCL platform
    errNum=clGetPlatformIDs(1, &firstPlatformID, &numPlatforms);
    if (errNum!=CL_SUCCESS||numPlatforms<=0)
    {
        cerr<<"Failed to find any OpenCL platforms."<<endl;
        return NULL;
    }

    //Create an OpenCL context on the platform.
    //Default to GPU context first, CPU context next
    cl_context_properties contextProperties[]=
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties) firstPlatformID,
        0
	};

    context=clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
    
    if (errNum!=CL_SUCCESS)
    {
        cerr<<"Failed to create an OpenCL GPU context, trying CPU."<<endl;
        context=clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
        if (errNum!=CL_SUCCESS)
        {
            cerr<<"Failed to create an OpenCL GPU or CPU context."<<endl;
            return NULL;
        }
	}
    return context;
}

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
    cl_int errNum;
    cl_device_id *devices;
    cl_command_queue commandQueue=NULL;
    size_t deviceBufferSize=-1;

    //Get the size of the devices buffer
    errNum=clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    
    if (errNum!=CL_SUCCESS)
    {
    	cerr<<"Failed call to clGetContextInfo()";
        return NULL;
    }
    if (deviceBufferSize<=0)
    {
        cerr<<"No devices available."<<endl;
        return NULL;
    }
    
    //Allocate memory for the devices buffer
    devices=new cl_device_id[deviceBufferSize/sizeof(cl_device_id)];
    errNum=clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
    if (errNum!=CL_SUCCESS)
    {
        cerr<<"Failed to get device IDs.";
        delete []devices;
        return NULL;
	}

    //Choose the first available device
    commandQueue=clCreateCommandQueue(context, devices[0], 0, NULL);
    if (commandQueue==NULL)
    {
        cerr<<"Failed to create commandQueue for device 0";
        return NULL;
    }
    *device=devices[0];
    delete []devices;
    return commandQueue;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char *fileName)
{
    cl_int errNum;
    cl_program program;
    ifstream kernelFile(fileName, ios::in);

    if (!kernelFile.is_open())
    {
        cerr<<"Failed to open file for reading:"<<fileName<<endl;
		return NULL;
    }

	ostringstream oss;
    oss<<kernelFile.rdbuf();
    string srcStdStr=oss.str();
    const char *srcStr=srcStdStr.c_str();
    program=clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
    
    if (program==NULL)
    {
        cerr<<"Failed to create CL program from source."<<endl;
        return NULL;
    }

    errNum=clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (errNum!=CL_SUCCESS)
    {
        //Determine the reason for the error
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        cerr<<"Error in kernel: "<<endl;
        cerr<<buildLog;
        clReleaseProgram(program);
        return NULL;
	}
    return program;
}

bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float *a, float *b)
{
    memObjects[0]=clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, a, NULL);
    memObjects[1]=clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, b, NULL);
    memObjects[2]=clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float)*ARRAY_SIZE, NULL, NULL);
    if (memObjects[0]==NULL || memObjects[1]==NULL || memObjects[2]==NULL)
    {
        cerr<<"Error creating memory objects."<<endl;
        return FALSE;
    }
    return TRUE;
}

void Cleanup(cl_context context, cl_command_queue commandQueue,              cl_program program, cl_kernel kernel, cl_mem memObjects[3])
{
    for (int i=0; i<3; i++)
    {
        if (memObjects[i]!=0)
            clReleaseMemObject(memObjects[i]);
    }
    if (commandQueue!=0)
        clReleaseCommandQueue(commandQueue);
    if (kernel!=0)
        clReleaseKernel(kernel);
    if (program!=0)
        clReleaseProgram(program);
    if (context!=0)
        clReleaseContext(context);
}