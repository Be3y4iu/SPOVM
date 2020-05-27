LD_LIBRARY_PATH = /home/ygenks/programming/4_semester/SP/5/lib.so
TARGET = lab5
CC = gcc
CFLAGS = -lpthread -lrt 
FLAGS = $(OPTIMIZATION) $(WARNINGS) $(CFLAGS)
OPTIMIZATION = 
WARNINGS = -Wshadow -Wall -Wextra -Wpedantic -Wbad-function-cast -Wcast-qual
DEPS = Makefile 
OBJ = main.o $(LD_LIBRARY_PATH) lib.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) 

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)



clean:
	rm -f $(TARGET) *.o
