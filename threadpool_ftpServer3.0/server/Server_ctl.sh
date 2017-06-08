#!/bin/bash

ROOT_PATH=$(pwd)
CONF=$ROOT_PATH/conf/server.conf
SERVER=$ROOT_PATH/ftpServer
name=$(basename $SERVER)
function Usage()
{
	echo -e "$0 ([-s|start] | [-t|stop] | [-rt|restart]"
}

function Start_server()
{
	pidof $SERVER
	if [ $? -eq 1 ];then
		IP=$(awk -F: '/IP/{print $2}' $CONF)
		PORT=$(awk -F: '/PORT/{print $2}' $CONF)
		./$name $IP $PORT
	fi
	echo -e "server is ready"
}

function Stop_server()
{
	pid=$(pidof $SERVER)
	if [ $? -eq 0 ];then
		kill -9 $pid
	fi
	echo -e "server is stop"
}

function Restart_server()
{
	Stop_server
	Start_server
}

 
if [ $# -ne 1 ];then
	Usage
	exit 1 
fi

case $1 in
	"-s"|"start")
		Start_server	
	;;
	"-t"|"stop")
		Stop_server
	;;
	"-rt"|"restart")
		Restart_server
	;;
	*)
		Usage
	;;
esac
