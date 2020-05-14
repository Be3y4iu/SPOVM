TARGET = lab4
CC = gcc
CFLAGS = -lpthread  
FLAGS = $(OPTIMIZATION) $(WARNINGS) $(CFLAGS)
OPTIMIZATION = 
WARNINGS = -Wshadow -Wall -Wextra -Wpedantic -Wbad-function-cast -Wcast-qual
DEPS = Makefile 
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS) 

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

clean:
	rm -f $(TARGET) *.o
