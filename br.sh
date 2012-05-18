#!/bin/sh

# br.sh [norun]
# builds the broswer code and then, if no arguments are specified, runs it

gcc net.c browser.c -o browser.run
if [ $? -eq 0 ]; then
 ./browser.run
fi

