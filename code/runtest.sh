#!/bin/bash

#step2
#COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/etape2'
#step3
COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/makethreads'


rm make_error.log 2> /dev/null

if [[ $1 = "clean" ]]; then
   echo "Clean..."
   make clean 2> make_error.log 1> /dev/null
   echo "Compilation en cours..."
   make  2>> make_error.log 1> /dev/null
else
   echo "Compilation en cours..."
   make 2> make_error.log 1> /dev/null
fi

if test -s "make_error.log"; then
    echo "Erreur :("
    echo "---------"
    cat make_error.log
else
    echo "Lancement du programme :)"
    echo "-------------------------"
    echo "$COMMAND"
    $COMMAND
fi
rm make_error.log 2> /dev/null
