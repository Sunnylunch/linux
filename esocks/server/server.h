#ifndef _SERVER_H__
#define _SERVER_H__

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

#include"protocol.h"
#include"common.h"
#include"transfer.h"
using namespace std;

void* handler(void *arg);

#endif //_SERVER_H__
