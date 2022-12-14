#!/bin/bash

make clean
make


./diff text1 text2 > output1

diff text1 text2 > output2

# echo "$output1"

if [ $(diff -s output1 output2 | wc -l) -eq 1 ]
then
	echo -e "\nTest cases passed!"
else
	echo -e "\nFailed to Pass test cases!"
    make clean
    exit 1
fi

[ -f output1 ] && rm output1
[ -f output2 ] && rm output2
