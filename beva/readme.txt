This is a simple readme for the BEVA binary code usage.

The code is compiled on Ubuntu 18.04.1 Bionic Beaver (Final) 
You can download the virtualmachine from https://www.osboxes.org/ubuntu/


To run the code, you can simply type:

bash run.sh imdb 1 1 100

It will output the result into the result directy.


The output format:

/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 7 FETR: 1 STUP: 0 FETD: 2 STUPD: 0 #RST: 9 #ACT: 3 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000000 STUTM: 0.000000 QRYTM: 0.003840
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 6 FETR: 1 STUP: 0 FETD: 47 STUPD: 0 #RST: 142 #ACT: 48 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000017 STUTM: 0.000000 QRYTM: 0.004705
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 5 FETR: 1 STUP: 0 FETD: 85 STUPD: 0 #RST: 430 #ACT: 86 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000007 STUTM: 0.000000 QRYTM: 0.003647
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 4 FETR: 1 STUP: 0 FETD: 363 STUPD: 0 #RST: 7341 #ACT: 364 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000078 STUTM: 0.000000 QRYTM: 0.003694
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 3 FETR: 1 STUP: 0 FETD: 551 STUPD: 0 #RST: 86673 #ACT: 552 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000397 STUTM: 0.000000 QRYTM: 0.001169
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 2 FETR: 1 STUP: 0 FETD: 351207 STUPD: 0 #RST: 351207 #ACT: 1 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.001160 STUTM: 0.000000 QRYTM: 0.000001
/home/jqin/Projects/fastssplus/source_beva_release/package/data/imdb.words.txt Algo: lazy_dfs tau: 2 #QRY: 1 #Trie: map PFXL: 1 FETR: 1 STUP: 0 FETD: 351207 STUPD: 0 #RST: 351207 #ACT: 1 #REALACT: 0 Idxtm: 0.708181 FCHTM: 0.000850 STUTM: 0.000000 QRYTM: 0.000001

Each line represents the statistics of one output prefix. We did not output the output string for simplicity reasons. 

Meaning by column:

data file name.
Algo: algorithm name.
tau: threshold.
#QRY: number of query.
#Trie: trie data structure.
PFXL: query prefix length.
FETR: ignore.
FETD: Node fetched.
STUPD: ignore.
#RST: number of results.
#ACT: number of active nodes.
#REALACT: ignore.
Idxtm: index build time.
FCHTM: time used to fetch result.
STUTM: ignore.
QRYTM: time used to perform the search. 


