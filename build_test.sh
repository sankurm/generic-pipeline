#!/bin/bash

PREFIX="$1"

DIR=`echo ${PREFIX} | cut -d'.' -f1`
FILE=`echo ${PREFIX} | cut -d'.' -f2`

HOME=..

if [ ${DIR} -eq 1 ]; then
    CXXSTD="-std=c++11"
elif [ ${DIR} -eq 2 ]; then
    CXXSTD="-std=c++17"
elif [ ${DIR} -eq 3 ]; then
    CXXSTD="-std=c++20"
fi

#Compile
echo -e "\033[0;33m$ g++ ${CXXSTD} -Wall -Werror ${HOME}/${DIR}_*/src/${PREFIX}_*.cpp -o ./${PREFIX}.out\033[0m"
g++ ${CXXSTD} -Wall -Werror ${HOME}/${DIR}_*/src/${PREFIX}_*.cpp -o ./${PREFIX}.out

status=$?
if [ ${status} -ne 0 ]; then
    exit ${status}
fi

#Run
echo -e "\033[0;33m$ ./${PREFIX}.out\033[0m"
./${PREFIX}.out
