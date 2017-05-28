#!/bin/bash

LOCAL_PATH=$(pwd)
SRC=$(ls ${LOCAL_PATH}/*.cpp | tr '\n' ' ')
CC=g++
LIB="-lpthread" 

cat <<EOF >Makefile
thttpd:${SRC} 
	$CC -o \$@ \$^ $LIB 

.PHONY:output
output:
	mkdir output
	cp -rf log output/
	cp -rf conf output/
	cp thttpd output/
	cp -rf wwwroot output/
	cp thttp_ctl.sh output/
	mkdir output/wwwroot/cgi_bin
	cp -rf cgi_bin/cgi_math output/wwwroot/cgi_bin/
.PHONY:clean
clean:
	rm -rf thttpd *.o output
EOF
