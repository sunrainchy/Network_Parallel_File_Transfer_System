EXE=main
AIM=global_fun.o oss_show.o
CC=g++ 
LIB= -losscore -lcurl
all: $(EXE)
$(EXE): main.cpp $(AIM)
	$(CC) -o $(EXE) $(AIM) main.cpp $(LIB) 
global_fun.o: global_fun.cpp global_fun.h
	$(CC) -c -o global_fun.o  global_fun.cpp
oss_show.o: oss_show.cpp oss_show.h
	$(CC) -c -o oss_show.o oss_show.cpp
clean: 
	rm $(EXE) $(AIM)
run: $(EXE)
	./$(EXE)
