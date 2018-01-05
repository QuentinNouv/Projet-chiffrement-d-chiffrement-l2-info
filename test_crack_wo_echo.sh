#!/bin/bash

function test_crack1 {
# critère C1
    while read k
    do
    while read i
    do
    ./xorcipher -i tests/crypted_crack/${k}_${i} -m 1 -l ${#k} > tests/keys0/${k}_${i}
    done < ./tests/file_list_crack.txt
    done  < ./tests/keys.txt
  }


    function test_crack2 {
    # critère C2
        while read k
        do
        while read i
        do
        ./xorcipher -i tests/crypted_crack/${k}_${i} -m 2 -l ${#k} > tests/keys1/${k}_${i}
        done < ./tests/file_list_crack.txt
        done  < ./tests/keys.txt

    }


    function test_crack31 {
    # critère C3.1
        while read k
        do
        while read i
        do
        ./xorcipher -i tests/crypted_crack/${k}_${i} -m 3.1 -l ${#k} > tests/keys2/${k}_${i}
        done < ./tests/file_list_crack.txt
        done  < ./tests/keys.txt
    }


    function test_crack32 {
        for i in 1 2 3
        do
        ./xorcipher -i tests/crypted_crack/adijd_mini.txt -m $i > tests/keys3/$i
        done
    }

    function test {
        test_$1
    }

#test crack1; # validité
#test crack2; #
test crack31;
#test crack32;

    exit 0
