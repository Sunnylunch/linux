DIR=-I/usr/include/mysql
LIB=-L/usr/lib/mysql
lib=-lmysqlclient
DEFINE=-DDEBUG
CC=g++

.PHONY:all
all:msg_insert msg_select msg_delete msg_update

msg_insert:msg_insert.cpp sql_api.cpp
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

msg_select:msg_select.cpp sql_api.cpp 
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

msg_delete:msg_delete.cpp sql_api.cpp 
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

msg_update:msg_update.cpp sql_api.cpp
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

.PHONY:clean
clean:
	rm -rf msg_insert msg_select msg_delete msg_update
