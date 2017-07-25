OBJ=main.cpp skiplist.cpp
CC=g++
SRC=skiplist

$(SRC):$(OBJ) skiplist.h
	$(CC) -o $@ $^  

.PHONY:clean
clean:
	rm -rf $(SRC)
