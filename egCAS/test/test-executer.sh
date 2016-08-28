#!/bin/bash -e


if [ -z $1 ]; then
        echo "no target directory given"
        echo "exiting..."
        exit 2
fi

if [ -z $2 ]; then
        echo "no arguments given"
        echo "exiting..."
        exit 1
fi


#iterate over all arguments of the script
for arg in "$@"
do
        FILE_TO_COPY=$(find . -name "$arg")
        if [ -n "${FILE_TO_COPY}" ]; then
                "${FILE_TO_COPY}"
        fi
        
done


echo -e "\e[32m==============================================\e[0m"
echo -e "\e[32m         all tests executed successful\e[0m"
echo -e "\e[32m==============================================\e[0m"
