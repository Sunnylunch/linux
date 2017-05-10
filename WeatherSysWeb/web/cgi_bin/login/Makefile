DIR=-I/usr/include/mysql
LIB=-L/usr/lib/mysql
lib=-lmysqlclient
DEFINE=-DDEBUG
CC=g++

login:login.cpp sql_api.cpp
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

.PHONY:clean
clean:
	rm -rf  login
