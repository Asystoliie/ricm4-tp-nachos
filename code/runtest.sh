#!/bin/bash

#step2
#COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/etape2'
#step3
#COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/makethreads'
#COMMAND2='./build-origin/nachos-userprog -rs 1 -x ./build/jointhreads'
#step 4
#COMMAND='./build-origin/nachos-userprog -rs 1 -x ./build/userpages'
COMMAND2='./build-origin/nachos-userprog -rs 1 -x ./build/forkprocess'

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
    echo "Lancement du Test 1 :)"
    echo "----------------------"
    echo "$COMMAND"
    $COMMAND
    echo "$COMMAND2"
    $COMMAND2

fi
rm make_error.log 2> /dev/null

