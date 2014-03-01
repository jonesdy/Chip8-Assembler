CC=g++
CFLAGS=-Wall -c
EXECUTABLE=c8asm

all:main.o CodeLabel.o Assembler.o
	$(CC) -o $(EXECUTABLE) $^

%.o:%.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *o $(EXECUTABLE)
