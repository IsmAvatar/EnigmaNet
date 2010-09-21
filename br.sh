#!/bin/sh

gcc browser.c -o browser
if [ $? -eq 0 ]; then
 ./browser
fi

