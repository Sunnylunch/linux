#!/bin/bash

ROOT_PATH=$(pwd)
conf=$ROOT_PATH/cli_conf/client.conf
src=$ROOT_PATH/cli
name=$(basename $src)


function Usage()
{
	echo "Usage:$0 [-s(start) | -t(stop) | -rt(restart)]"
}

function start_client()
{
	$(pidof $name)
	if [ $? -eq 1 ];then
		local_port=$(awk -F: '/LOCAL_PORT/{print $2}' $conf)
		server_ip=$(awk -F: '/SERVER_IP/{print $2}' $conf)
		server_port=$(awk -F: '/SERVER_PORT/{print $2}' $conf)
		./$name $local_port $server_ip $server_port
	fi
	echo "client is ready running"
}

function stop_client()
{
	pid=$(pidof $name)
	if [ $? -eq 0 ];then
		kill -9 $pid
	fi
	echo "client is stop"
}

function restart_client()
{
	stop_client
	start_client
}

if [ $# -ne 1 ];then
	Usage
	exit 1
fi

case $1 in
'-s'|'start')
	start_client
	;;
'-t'|'stop')
	stop_client
	;;
'-rt'|'restart')
	restart_client
	;;
	*)
	Usage
	;;
esac
