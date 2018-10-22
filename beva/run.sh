#!/bin/bash

# This is a sample code to run the test.


#!/bin/bash


if [ ! ${#} -eq "5" ];then
    echo "Usage dataset mint maxt qnum"
    exit -1
fi

DATA=$1
RAWDATA=$1.words.txt
MINT=$2
MAXT=$3
TSTEP=1
QRYNUM=$4
QRYSIZE=$5

WORKDIR=`pwd`

BINDIR="$WORKDIR/"
DATADIR="$WORKDIR/data"
QUERYDIR="$WORKDIR/data"
RESULTDIR="$WORKDIR/lazyresults/query/"
EDITDFADIR="$WORKDIR/eva_conf"
mkdir -p ${RESULTDIR}/${DATA}/

for((i=$MINT;i<=$MAXT;i=i+$TSTEP))
do 
    echo "$ALGO, $DATA, $QRYNUM $q $i"
    QUERYFILE="$QUERYDIR/$RAWDATA.$QRYSIZE.query.txt"


    head -n $QRYNUM $QUERYFILE | $BINDIR/searcher_base_test -t $i -d $DATADIR/$RAWDATA -a lazy_dfs  -u     -o eva    -f -k 1 1>/dev/null  2>${RESULTDIR}/${DATA}/$RAWDATA.lazy_dfs_u.t$i.com.log
    
    # head -n $QRYNUM $QUERYFILE | $BINDIR/searcher_base_test -t $i -d $DATADIR/$RAWDATA -a patricia  -u     -o eva    -f -k 1 1>/dev/null  2>${RESULTDIR}/${DATA}/$RAWDATA.patricia_u.t$i.com.log

    # head -n $QRYNUM $QUERYFILE | $BINDIR/searcher_base_test -t $i -d $DATADIR/$RAWDATA -a lazy_dfs      -u  -o upeva  -f -k 1 1>/dev/null  2>${RESULTDIR}/${DATA}/$RAWDATA.lazy_dfs_u.t$i.upeva.log

done

exit 0

