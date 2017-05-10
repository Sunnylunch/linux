#!/bin/bash
name=$1
air=$2
Mail=$3
echo $name+$air | mail -s Weather $Mail
