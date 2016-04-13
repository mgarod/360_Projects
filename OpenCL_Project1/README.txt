Author: Michael Garod
Date: 4/12/16
Class: CSCI 360
Professor: Adriana Wise
Assignement:
	1. Install OpenCL on your platform
	2. Copy the 4 source code files from today’s lecture, preserving the file structure,
	and create a project within your IDE (Xcode, CMake GUI etc.)
	3. Compile and run
	4. Change the code to:
		(a) increase the size of the vectors to 1000
		(b) read the vectors from a file (you can generate them separately)
		(c) perform vector multiplication, instead of addition
-------------------------------------------------------------------------------

Build With:
	- make all
	- make run
	- make clean

Assumptions:
	- Matrix multiplication means multiply simply changing:
		A[i]+B[i] to A[i]*B[i]

Notes:
	- A text file called vectors.txt is generated before building the vectors
	- When the vectors are filled, they are filled with values from vectors.txt
	- Results of the matrix multiplication is output to the console as well as a text file called result.txt
	- In HelloWorld.cpp, line 35, the literal string was modified to work on my local machine