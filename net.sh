#!/bin/sh

# net.sh [server|client]
# builds both and then, if specified, runs as either a server or client

gcc -o server server.c net.c
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

