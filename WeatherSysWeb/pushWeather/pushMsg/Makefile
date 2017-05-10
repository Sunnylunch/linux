DIR=-I/usr/include/mysql
LIB=-L/usr/lib/mysql
lib=-lmysqlclient
DEFINE=-DDEBUG
CC=g++

sendWeather:main.cpp UserMsg.cpp weather.cpp
	$(CC) -o $@  $^ $(LIB) $(lib) $(DIR)

.PHONY:clean
clean:
	rm -rf sendWeather
