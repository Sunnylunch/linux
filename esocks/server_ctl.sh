#!/bin/bash

ROOT_PATH=$(pwd)
conf=$ROOT_PATH/ser_conf/server.conf
src=$ROOT_PATH/ser
name=$(basename $src)


function Usage()
{
	echo "Usage:$0 [-s(start) | -t(stop) | -rt(restart)]"
}

function start_server()
{
	$(pidof $name)
	if [ $? -eq 1 ];then
		server_port=$(awk -F: '/PORT/{print $2}' $conf)
		./$name $server_port
	fi
	echo "server is ready running"
}

function stop_server()
{
	pid=$(pidof $name)
	if [ $? -eq 0 ];then
		kill -9 $pid
	fi
	echo "server is stop"
}

function restart_server()
{
	stop_server
	start_server
}

if [ $# -ne 1 ];then
	Usage
	exit 1
fi

case $1 in
'-s'|'start')
	start_server
	;;
'-t'|'stop')
	stop_server
	;;
'-rt'|'restart')
	restart_server
	;;
	*)
	Usage
	;;
esac
