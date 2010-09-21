#!/bin/sh

# br.sh [norun]
# builds the broswer code and then, if no arguments are specified, runs it

gcc browser.c -o browser
if [ $? -eq 0 ]; then
 ./browser
fi

