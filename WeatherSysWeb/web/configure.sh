#!/bin/bash

LOCAL_PATH=$(pwd)
SRC=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' ')
OBJ=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' '|sed 's/\.c/\.o/g')
CC=g++
INCLUDE="-lpthread -lcolib -ldl"

#thttpd
cat <<EOF >Makefile

ROOT_PATH=\$(shell pwd)
INCLUDE=-I\$(ROOT_PATH)/libco
LIB=-L\$(ROOT_PATH)/libco/solib

thttpd:${OBJ}
	$CC -o \$@ \$< $INCLUDE \$(LIB) 
%.o:%.c
	$CC -c \$< \$(INCLUDE)

.PHONY:output
output:
	mkdir output
	cp -rf log output/
	cp -rf conf output/
	cp thttpd output/
	cp -rf wwwroot output/
	cp thttp_ctl.sh output/
	mkdir -p output/wwwroot/cgi_bin/sql
	cp -rf cgi_bin/login/login  output/wwwroot/cgi_bin/sql
	cp -rf cgi_bin/register/register  output/wwwroot/cgi_bin/sql/
	cp -rf cgi_bin/msg/msg_insert  output/wwwroot/cgi_bin/sql/
	cp -rf cgi_bin/msg/msg_delete  output/wwwroot/cgi_bin/sql/
	cp -rf cgi_bin/msg/msg_update  output/wwwroot/cgi_bin/sql/
	cp -rf cgi_bin/msg/msg_select  output/wwwroot/cgi_bin/sql/
.PHONY:clean
clean:
	rm -rf thttpd *.o output
EOF
