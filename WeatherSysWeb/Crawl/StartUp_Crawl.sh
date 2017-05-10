#!/bin/bash

ROOT_PATH=$(pwd)
CRAWL_PATH=$ROOT_PATH/CrawlWeather/weather
cmd="scrapy crawl weather"

$ROOT_PATH/emptySql.py

cd $CRAWL_PATH

sleep 1
$cmd

cd -


