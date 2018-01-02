#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

BASE=./
TEST=./tests
RET=

function test_crack1 {
# critère C1
    if [ -x $BASE/xorcipher ]
    then
    while read k
    do
    while read i
    do
    $BASE/xorcipher -i $TEST/crypted_crack/${k}_${i} -m 1 -l ${#k} > $TEST/keys0/${k}_${i}
    done < ./$TEST/file_list_crack.txt
    done  < ./$TEST/keys.txt
    fi
    }


    function test_crack2 {
    # critère C2
        if [ -x $BASE/xorcipher ]
        then
        while read k
        do
        while read i
        do
        $BASE/xorcipher -i $TEST/crypted_crack/${k}_${i} -m 2 -l ${#k} > $TEST/keys1/${k}_${i}
        done < ./$TEST/file_list_crack.txt
        done  < ./$TEST/keys.txt
        RET=0
        else
        RET=2
        fi

    }


    function test_crack31 {
    # critère C3.1
        rm -rf $TEST/keys2
        mkdir $TEST/keys2

        if [ -x $BASE/xorcipher ]
        then
        while read k
        do
        while read i
        do
        $BASE/xorcipher -i $TEST/crypted_crack/${k}_${i} -m 3.1 -l ${#k} > $TEST/keys2/${k}_${i}
        done < ./$TEST/file_list_crack.txt
        done  < ./$TEST/keys.txt
        else
        RET=2
        fi

    }


    function test_crack32 {

        if [ -x $BASE/xorcipher ]
        then
        for i in 1 2 3
        do
        $BASE/xorcipher -i $TEST/crypted_crack/adijd_mini.txt -m $i > $TEST/keys3/$i
        done
        fi

    }

    function test {
        test_$1
    }

#test crack1; # validité
#test crack2; #
test crack31;
#test crack32;

    exit 0
