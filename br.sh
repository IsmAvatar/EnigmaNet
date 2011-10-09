#!/bin/sh

# br.sh [norun]
# builds the broswer code and then, if no arguments are specified, runs it

gcc -o browser browser.c net.c
if [ $? -eq 0 ]; then
 ./browser
fi

