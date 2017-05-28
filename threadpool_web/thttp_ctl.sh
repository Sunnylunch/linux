#!/bin/bash

LOCAL_PATH=$(pwd)
BIN=${LOCAL_PATH}/thttpd
CONF=${LOCAL_PATH}/conf/thttpd.conf

function thttpd_start()
{
	[ -f thttpd.pid ] &&{
		printf "thttpd is exist...\n"
		return 
	}
	IP=`grep -E '^IP:' $CONF | awk -F: '{print $2}' `
	PORT=`grep -E '^PORT:' $CONF | awk -F: '{print $2}' `
	$BIN $IP $PORT
	pidof thttpd >thttpd.pid
	printf "thttpd is runing...\n"
}

function thttpd_stop()
{
	[ ! -f thttpd.pid ] && {
		printf "thttpd not is exist...\n"
		return 
	}
	PID=$(cat thttpd.pid)
	kill -9 $PID
	rm -rf thttpd.pid
	printf "thttpd is dead...\n"
}

function usage()
{
	printf "%s [start(-s) | stop(-t) | restart(-rt)]\n" "$0"
}

[ $# -ne 1 ] &&{
	usage
	exit 1
}

case $1 in
	start|-s)
	thttpd_start
	;;
	stop |-t)
	thttpd_stop
	;;
	restart|-rt)
	thttpd_stop
	thttpd_start
	;;
	*)
	usage
	;;
esac
