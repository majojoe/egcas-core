#!/bin/bash -e



if [ -z $1 ]; then
        echo "no arguments given"
        echo "exiting..."
fi

#iterate over all arguments of the script
for arg in "$@"
do
        echo $PWD "$arg"
done