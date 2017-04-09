#!/bin/bash

LOCAL_PATH=$(pwd)
SRC=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' ')
OBJ=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' '|sed 's/\.c/\.o/g')
CC=gcc
INCLUDE="-lpthread"
CGI_OBJ=$(ls ${LOCAL_PATH}/cgi_bin/*.c | tr '\n' ' ')


#cgi
cat <<EOF >${LOCAL_PATH}/cgi_bin/Makefile
cgi_math:$CGI_OBJ
	$CC -o \$@ \$^

.PHONY:clean
clean:
	rm -rf cgi_math
EOF


#thttpd
cat <<EOF >Makefile

.PHONY:all
all:thttpd
	cd cgi_bin;make;cd -

thttpd:${OBJ}
	$CC -o \$@ \$< $INCLUDE
%.o:%.c
	$CC -c \$<

.PHONY:output
output:
	mkdir output
	cp -rf log output/
	cp -rf conf output/
	cp thttpd output/
	cp -rf wwwroot output/
	cp thttp_ctl.sh output/
	mkdir -p output/wwwroot/cgi_bin
	cp cgi_bin/cgi_math output/wwwroot/cgi_bin

.PHONY:clean
clean:
	rm -rf thttpd *.o output;cd cgi_bin;make clean;cd -
EOF
