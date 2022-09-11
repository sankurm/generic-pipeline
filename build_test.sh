#!/bin/bash

PREFIX="$1"

DIR=`echo ${PREFIX} | cut -d'.' -f1`
FILE=`echo ${PREFIX} | cut -d'.' -f2`

HOME=..

if [ ${DIR} -eq 1 ]; then
    CXXSTD="-std=c++11"
elif [ ${DIR} -eq 2 ]; then
    CXXSTD="-std=c++17"
fi

#Compile
echo g++ ${CXXSTD} -Wall -Werror ${HOME}/${DIR}_*/src/${PREFIX}_*.cpp -o ./${PREFIX}.out
g++ ${CXXSTD} -Wall -Werror ${HOME}/${DIR}_*/src/${PREFIX}_*.cpp -o ./${PREFIX}.out

#Run
echo ./${PREFIX}.out
./${PREFIX}.out
