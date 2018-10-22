## Requirements -- How to run this script
# $ python3 beva.py {dataset} {mint} {maxt} {qryNumber}
import os
import sys

clusterRes = []

def clusteringResults(results):
    output = {'time': 0}
    dic = {}
    for param in range(0,len(results),2):
        # print(results[param], results[param+1])
        if(results[param] == '#RST:'):
            output['results'] = results[param+1]
        elif(results[param] == 'FCHTM:' or results[param] == 'QRYTM:'):
            output['time'] += float(results[param+1])
    return output

def lazyDFS(params):
    bashCommand = f'bash run.sh {params["dataset"]} {params["mint"]} {params["maxt"]} {params["qryNumber"]} {params["qrySize"]} 1> output.txt 2> error.txt'
    os.system(bashCommand)

    print(f'[INFO] Reading Query {params["qryNumber"]}')
    for tau in range(params['mint'], params['maxt']+1):

        fnameDfs = f'lazyresults/query/{params["dataset"]}/{params["dataset"]}.words.txt.lazy_dfs_u.t{tau}.com.log'
        # print(f'[INFO] Reading {fnameDfs} for tau {tau}')

        with open(fnameDfs) as f:
            content = f.readlines()
            # splitted = []
            # for x in content:
            #     line = x.strip()
            #     splitted.append(line.split(' '))
            output = clusteringResults(content[0].strip().split()[1:])
        f.close()
        query = {
            'tau': tau,
            'algorithm': 'lazy_dfs',
            'qtd_queries': params['qryNumber'],
            'total_time_ms': output['time']*1000,
            'avg_time_ms' : (output['time']*1000)/params['qryNumber'],
            'results': output['results']
        }
        print(query)
        clusterRes.append(query)
def main():
    print('### Starting beva process ###')
    print()
    params = {}
    params['dataset'] = sys.argv[1]
    params['mint'] = int(sys.argv[2])
    params['maxt'] = int(sys.argv[3])
    minQry = int(sys.argv[4])
    maxQry = int(sys.argv[5])
    params['qrySize'] = int(sys.argv[6])
    # for qi in range(minQry,maxQry+1):
    qi = 1000
    params['qryNumber'] = qi
    lazyDFS(params)

    # TODO:: calculateMean
    # TODO:: write results

    print()
    print('### Finishing beva process ###')

if __name__ == "__main__":
    if(len(sys.argv) == 7):
        main()
    else:
        print('bash run.sh {dataset} {mint} {maxt} {minQry} {maxQry} {sizeQry}')
