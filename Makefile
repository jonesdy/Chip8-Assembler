CC=g++
CFLAGS=-Wall -c
EXECUTABLE=c8asm
SFML_INCLUDES=-I./$(SFML_FOLDER)/include
SFML_LIBS=-lsfml-system -lsfml-window -lsfml-graphics

all:main.o CodeLabel.o Assembler.o
	$(CC) -o $(EXECUTABLE) $^ $(SFML_LIBS)

%.o:%.cpp
	$(CC) $(CFLAGS) $(SFML_INCLUDES) $<

clean:
	rm -rf *o $(EXECUTABLE)
