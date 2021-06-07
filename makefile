# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -std=c++17 -Ofast -m64
INCLUDES = -I/opt/homebrew/include -D_THREAD_SAFE
LFLAGS = -L/opt/homebrew/lib -lSDL2 -lpthread

# the build target executable:
TARGET = bin/marcher
SRC = src/main.cpp

all:
	$(CC) $(INCLUDES) $(CFLAGS) -o $(TARGET) $(SRC) $(LFLAGS)

clean:
	$(RM) $(TARGET)