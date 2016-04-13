#include <iostream>
#include <fstream>
#include "functions.h"
#include <OpenCL/opencl.h>
#define ARRAY_SIZE 1000
using namespace std;

int main(int argc, char **argv)
{
    cl_context context=0;
    cl_command_queue commandQueue=0;
    cl_program program=0;
    cl_device_id device=0;
    cl_kernel kernel=0;
    cl_mem memObjects[3]={0, 0, 0};
    cl_int errNum;

    //Create an OpenCL context on first available platform
    context=CreateContext();
    if (context==NULL)
    {
        cerr<<"Failed to create OpenCL context."<<endl;
		return 1;
	}

    //Create a command queue on first available device
    commandQueue=CreateCommandQueue(context, &device);
    if (commandQueue==NULL)
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Create program from "HelloWorld.cl" kernel source
    program=CreateProgram(context, device, "/Users/Mike/Desktop/OpenCL/HW1/HelloWorld.cl");
    if (program==NULL)
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Create OpenCL kernel
    kernel=clCreateKernel(program, "hello_kernel", NULL);
    if (kernel==NULL)
    {
        cerr<<"Failed to create kernel."<<endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Create memory objects that will be used as arguments to
	//kernel. First create host memory arrays that will store the
	//arguments to kernel
    float result[ARRAY_SIZE];
    float a[ARRAY_SIZE];
    float b[ARRAY_SIZE];

    // Create a file containing vector data
    ofstream outputfile("vectors.txt");
    for (int i=0; i<ARRAY_SIZE; i++)
    {
        outputfile << (float)i << " " << 2*(float)i << endl;
    }

    // Construct the vectors from the file containing vector data
    ifstream inputfile("vectors.txt");
    for (int i=0; i<ARRAY_SIZE; i++)
    {
        float f;
        inputfile >> f;
        a[i] = f;
        cout<<"a["<<i<<"]="<<a[i]<<endl;
        
        inputfile >> f;
        b[i] = f;
        cout<<"b["<<i<<"]="<<b[i]<<endl;
    }

    if (CreateMemObjects(context, memObjects, a, b)==FALSE)
    {
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Set the kernel arguments
    errNum=clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
    errNum|=clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
    errNum|=clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
    if (errNum!=CL_SUCCESS)
    {
        cerr<<"Error setting kernel arguments."<<endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
    size_t globalWorkSize[1]={ARRAY_SIZE};
    size_t localWorkSize[1]={1};

    //Queue the kernel up for execution across the array
    errNum=clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    if (errNum!=CL_SUCCESS)
    {
        cerr<<"Error ."<<endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Transfer the result from buffer back to host
    errNum=clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, ARRAY_SIZE*sizeof(float), result, 0, NULL, NULL);
    if (errNum!=CL_SUCCESS)
    {
    	cerr<<"Error reading result buffer."<<endl;
        Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

    //Output the result buffer
    outputfile.close();
    outputfile.open("result.txt");
    for (int i=0; i<ARRAY_SIZE; i++)
    {
        cout<<result[i]<<" ";
        outputfile << result[i] << endl;
    }
    cout<<endl;

    cout<<"Executed program successfully."<<endl;
    Cleanup(context, commandQueue, program, kernel, memObjects);
    return 0;
}