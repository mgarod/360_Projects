CC=c++
DEBUG=-g
CFLAGS=-Wall -c $(DEBUG)

INCS=-I/System/Library/Frameworks/OpenCL.framework/Headers
LIBS=-F/System/Library/Frameworks -framework OpenCL

all:
	make HelloWorld

run:
	./HelloWorld

HelloWorld.o: HelloWorld.cpp
	$(CC) $(CFLAGS) $(INCS) -o HelloWorld.o HelloWorld.cpp

functions.o: functions.cpp
	$(CC) $(CFLAGS) $(INCS) -o functions.o functions.cpp

HelloWorld: HelloWorld.cpp HelloWorld.o functions.o
	$(CC) HelloWorld.o functions.o $(LIBS) -o HelloWorld

clean:
	rm *.o
	rm HelloWorld
	rm vectors.txt
	rm result.txt