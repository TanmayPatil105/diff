#!/bin/bash


./diff text1 text2 > output1

diff text1 text2 > output2

if [ $(diff -s output1 output2 | wc -l) -eq 1 ]
then
    echo -e "\nTest cases Passed!!"
else  
    echo -e "\nTest cases Failed!!"
    exit 1
fi
