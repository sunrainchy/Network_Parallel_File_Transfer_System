EXE=main
AIM=global_fun.o
CC=gcc
LIB= -losscore -lcurl
all: $(EXE)
$(EXE): main.c $(AIM)
	gcc -o $(EXE) main.c $(AIM) $(LIB) 
global_fun.o: global_fun.c global_fun.h
	gcc -c global_fun.c -o global_fun.o
clean: 
	rm $(EXE) $(AIM)

run: $(EXE)
	./$(EXE)
