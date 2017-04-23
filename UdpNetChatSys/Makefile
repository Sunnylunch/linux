ROOT_PATH=$(shell pwd)
CLIENT_PATH=$(ROOT_PATH)/client
SERVER_PATH=$(ROOT_PATH)/server
CONF_PATH=$(ROOT_PATH)/conf
LIB_PATH=$(ROOT_PATH)/lib
LOG_PATH=$(ROOT_PATH)/log
WINDOW_PATH=$(ROOT_PATH)/window
CC=g++

SERVER=UdpServer
CLIENT=UdpClient

SERVER_SRC=$(shell ls $(SERVER_PATH) | egrep -v '.*\.h' | egrep '.*\.cpp')
CLIENT_SRC=$(shell ls $(CLIENT_PATH) | egrep -v '.*\.h' | egrep '.*\.cpp')

SERVER_OBJ=$(shell echo $(SERVER_SRC) | sed 's/\.cpp/\.o/g')
CLIENT_OBJ=$(shell echo $(CLIENT_SRC) | sed 's/\.cpp/\.o/g')

.PHONY:all
all:$(SERVER) $(CLIENT)

$(SERVER):$(SERVER_OBJ)
	@echo Link [$^] to [$@] ...
	$(CC) -o $@ $^
	@echo Link done...

$(CLIENT):$(CLIENT_OBJ)
	@echo Link [$^] to [$@] ...
	$(CC) -o $@ $^
	@echo Link done...

%.o:$(SERVER_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $<

%.o:$(CLIENT_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $<

.PHONY:clean
clean:
	rm -rf $(SERVER) $(CLIENT) *.o


.PHONY:debug
debug:
	@echo $(SERVER_SRC)
	@echo $(CLIENT_SRC)
	@echo $(SERVER_OBJ)
	@echo $(CLIENT_OBJ)



