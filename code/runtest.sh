#!/bin/bash

COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/makethreads'

rm make_error.log 2> /dev/null
make 2> make_error.log 1> /dev/null
if test ! -e "make_error.log"; then
    cat make_error.log
else
    $COMMAND
fi
rm make_error.log 2> /dev/null
