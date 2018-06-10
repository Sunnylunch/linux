DIR=-I/usr/include/mysql
LIB=-L/usr/lib/mysql
lib=-lmysqlclient
CC=g++

chess:ChessServer.cpp sql_api.cpp
	$(CC)  -o $@  $^ $(LIB) $(lib) $(DIR)

.PHONY:clean

clean:
	rm -rf chess
