EXE=shiyan
CC=gcc
LIB= -losscore -lcurl
all: $(EXE)
shiyan: shiyan.c
	gcc -o $(EXE) shiyan.c $(LIB)
clean: 
	rm $(EXE)

run: $(EXE)
	./$(EXE)
