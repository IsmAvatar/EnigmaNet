#!/bin/sh

# net.sh [server|client]
# builds both and then, if specified, runs as either a server or client

gcc net.c server.c -o server.run
if [ $? -eq 0 ]; then
 gcc net.c client.c -o client.run
fi
if [ $# -eq 1 ]; then
 if [ $1 = 'server' ]; then
  ./server.run
 fi
 if [ $1 = 'client' ]; then
  ./client.run
 fi
fi

