CC = gcc
FLAGS = -Wall -Wextra -g
OBJFILES = Main.o Literal.o
TARGET = main

all: $(TARGET)
Main.o: Main.c Main.h
	$(CC) $(FLAGS) -c Main.c
Literal.o: Literal.c Literal.h
	$(CC) $(FLAGS) -c Literal.c
$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(FLAGS)
clean:
	rm -f *.o $(TARGET) 