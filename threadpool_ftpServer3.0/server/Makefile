ROOT_PATH=$(shell pwd)
THREAD_POOL_PATH=$(ROOT_PATH)/pthread_pool
COMM_PATH=$(ROOT_PATH)/common

CC=g++
LIB=-lpthread
INCLUDE=-I$(COMM_PATH) $(THREAD_POOL_PATH)


SERVER=ftpServer
SRC=$(shell ls $(ROOT_PATH) $(COMM_PATH) $(THREAD_POOL_PATH) | egrep -v '.*\.h' | grep '.*\.cpp')
OBJ=$(shell echo $(SRC) | sed 's/\.cpp/\.o/g')

$(SERVER):$(OBJ)
	$(CC) -o $@ $^ $(LIB)
%.o:$(ROOT_PATH)/%.cpp
	$(CC) -c $< $(INCLUDE)
%.o:$(COMM_PATH)/%.cpp
	$(CC) -c $<
%.o:$(THREAD_POOL_PATH)/%.cpp
	$(CC) -c $<

.PHONY:clean
clean:
	rm -rf ftpServer *.o 

