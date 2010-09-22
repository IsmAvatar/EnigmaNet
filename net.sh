#!/bin/sh

gcc server.c -o server
if [ $? -eq 0 ]; then
 gcc client.c -o client
fi
if [ $# -eq 1 ]; then
 if [ $1 = 'server' ]; then
  ./server
 fi
 if [ $1 = 'client' ]; then
  ./client
 fi
fi

