ROOT_PATH=$(shell pwd)
COMM_PATH=$(ROOT_PATH)/common
SERVER_PATH=$(ROOT_PATH)/server
CLIENT_PATH=$(ROOT_PATH)/client

INCLUDE=-I$(COMM_PATH) $(SERVER_PATH) $(CLIENT_PATH)
LIB=-lpthread
CC=g++

SERVER_SRC=$(shell ls $(SERVER_PATH) $(COMM_PATH)| egrep -v ".*\.h" | egrep ".*\.cpp")
CLIENT_SRC=$(shell ls $(CLIENT_PATH) $(COMM_PATH) | egrep -v ".*\.h" | egrep ".*\.cpp")

SERVER_OBJ=$(shell echo $(SERVER_SRC) | sed 's/\.cpp/\.o/g')
CLIENT_OBJ=$(shell echo $(CLIENT_SRC) | sed 's/\.cpp/\.o/g')

SERVER=ser
CLIENT=cli

.PHONY:all
all:$(SERVER) $(CLIENT)

$(SERVER):$(SERVER_OBJ)
	$(CC) -o $@ $^ $(LIB)
$(CLIENT):$(CLIENT_OBJ)
	$(CC) -o $@ $^ $(LIB)

%.o:$(SERVER_PATH)/%.cpp
	$(CC) -c $<  $(INCLUDE)
%.o:$(CLIENT_PATH)/%.cpp
	$(CC) -c $<  $(INCLUDE)
%.o:$(COMM_PATH)/%.cpp
	$(CC) -c $<  $(INCLUDE)

.PHONY:output
output:
	mkdir output
	mkdir output/client
	mkdir output/server
	cp -rf cli output/client
	cp -rf client_ctl.sh output/client
	cp -rf cli_conf output/client
	cp -rf ser output/server
	cp -rf server_ctl.sh output/server
	cp -rf ser_conf output/server

.PHONY:clean
clean:
	rm -rf $(ROOT_PATH)/*.o $(SERVER) $(CLIENT) 
	rm -rf output

