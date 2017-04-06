#!/bin/bash

LOCAL_PATH=$(pwd)
SRC=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' ')
echo $SRC
OBJ=$(ls ${LOCAL_PATH}/*.c | tr '\n' ' '|sed 's/\.c/\.o/g')
CC=gcc
INCLUDE="-lpthread"

cat <<EOF >Makefile
thttpd:${OBJ}
	$CC -o \$@ \$< $INCLUDE
%.o:%.c
	$CC -c \$<
.PHONY:clean
clean:
	rm -rf thttpd *.o
EOF

