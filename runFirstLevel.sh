#!/bin/bash

rm result.txt

echo "----------------------------------------------------------------------------------------------"
echo "TWO LEVEL"
cd twolevel-n
echo "making twolevel-n"
make



for((j=4;j<=10;j=j+1))
do 

    echo -e "----------------------------------------------------------------------------------------------\n    $j CARACTERES\n----------------------------------------------------------------------------------------------" >> ../result.txt

    for((i=3;i<=13;i=i+2))
    do 
        echo "two-level - usaddr - q$i - tau:1 - index1: $j"
        ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 $j >> ../result.txt
    done


    for((i=3;i<=13;i=i+2))
    do 
        echo "two-level - usaddr - q$i - tau:2 - index1: $j"
        ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q${i}_10.txt 2 $j >> ../result.txt
    done


    for((i=3;i<=13;i=i+2))
    do 
        echo "two-level - usaddr - q$i - tau:3 - index1: $j"
        ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q${i}_10.txt 3 $j >> ../result.txt
    done
    
done



cd ..
