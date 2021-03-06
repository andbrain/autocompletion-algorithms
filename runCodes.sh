#!/bin/bash

rm result_codes.txt

echo "----------------------------------------------------------------------------------------------"
echo "TWO LEVEL 8"
cd twolevel-n
echo "making twolevel-8"
make

cat > ../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    TWO LEVEL 8
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - aol - q$i - tau:1 - index1: 8"
    ./twoltrie ~/aol/aol.txt ~/aol/q$i.txt 1 8 >> ../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - usaddr - q$i - tau:1 - index1: 8"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 8 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - aol - q3 - tau:$i - index1: 8"
    ./twoltrie ~/aol/aol.txt ~/aol/q3_10.txt $i 8 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - usaddr - q3 - tau:$i - index1: 8"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i 8 >> ../result_codes.txt
done

cd ..


echo "----------------------------------------------------------------------------------------------"
echo "TWO LEVEL 9"
cd twolevel-n
echo "making twolevel-9"
make

cat > ../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    TWO LEVEL 9
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - aol - q$i - tau:1 - index1: 9"
    ./twoltrie ~/aol/aol.txt ~/aol/q$i.txt 1 9 >> ../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - usaddr - q$i - tau:1 - index1: 9"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 9 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - aol - q3 - tau:$i - index1: 9"
    ./twoltrie ~/aol/aol.txt ~/aol/q3_10.txt $i 9 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - usaddr - q3 - tau:$i - index1: 9"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i 9 >> ../result_codes.txt
done

cd ..



echo "----------------------------------------------------------------------------------------------"
echo "TWO LEVEL 10"
cd twolevel-n
echo "making twolevel-10"
make

cat > ../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    TWO LEVEL 10
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - aol - q$i - tau:1 - index1: 10"
    ./twoltrie ~/aol/aol.txt ~/aol/q$i.txt 1 10 >> ../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "two-level - usaddr - q$i - tau:1 - index1: 10"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 10 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - aol - q3 - tau:$i - index1: 10"
    ./twoltrie ~/aol/aol.txt ~/aol/q3_10.txt $i 10 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "two-level - usaddr - q3 - tau:$i - index1: 10"
    ./twoltrie ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i 10 >> ../result_codes.txt
done

cd ..



echo "----------------------------------------------------------------------------------------------"
echo "META"
cd meta/threshold
echo "making meta"
make

cat >> ../../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    META
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "meta - aol - q$i - tau:1"
    ./sigmod15 ~/aol/aol.txt ~/aol/q$i.txt 1 >> ../../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "meta - usaddr - q$i - tau:1"
    ./sigmod15 ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 >> ../../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "meta - aol - q3 - tau:$i"
    ./sigmod15 ~/aol/aol.txt ~/aol/q3_10.txt $i >> ../../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "meta - usaddr - q3 - tau:$i"
    ./sigmod15 ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i >> ../../result_codes.txt
done

cd ../..


echo "----------------------------------------------------------------------------------------------"
echo "ICAN"
cd ican
echo "making ican"
make

cat >> ../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    ICAN
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "ican - aol - q$i - tau:1"
    ./ican ~/aol/aol.txt ~/aol/q$i.txt 1 >> ../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "ican - usaddr - q$i - tau:1"
    ./ican ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "ican - aol - q3 - tau:$i"
    ./ican ~/aol/aol.txt ~/aol/q3_10.txt $i >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "ican - usaddr - q3 - tau:$i"
    ./ican ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i >> ../result_codes.txt
done

cd ..


echo "----------------------------------------------------------------------------------------------"
echo "IPCAN"
cd ipcan
echo "making ipcan"
make

cat >> ../result_codes.txt <<EOF

----------------------------------------------------------------------------------------------
    IPCAN
----------------------------------------------------------------------------------------------
EOF

for((i=3;i<=13;i=i+2))
do 
    echo "ipcan - aol - q$i - tau:1"
    ./ipcan ~/aol/aol.txt ~/aol/q$i.txt 1 >> ../result_codes.txt
done

for((i=3;i<=13;i=i+2))
do 
    echo "ipcan - usaddr - q$i - tau:1"
    ./ipcan ~/usaddr/usaddr.txt ~/usaddr/q$i.txt 1 >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "ipcan - aol - q3 - tau:$i"
    ./ipcan ~/aol/aol.txt ~/aol/q3_10.txt $i >> ../result_codes.txt
done

for((i=1;i<=5;i=i+1))
do 
    echo "ipcan - usaddr - q3 - tau:$i"
    ./ipcan ~/usaddr/usaddr.txt ~/usaddr/q3_10.txt $i >> ../result_codes.txt
done

cd ..