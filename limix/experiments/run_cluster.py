import os

def run_jobs(dataset_name,seed):
    cmd  = "bsub -o ~/research/users/rakitsch/cluster_out -e ~/research/users/rakitsch/cluster_out "
    cmd += ' -R "rusage[mem=8000]" -M 8000 '
    cmd += "python test_lowerbound.py %s %d"%(dataset_name,seed)
    print cmd
    os.system(cmd)


for i in range(100):
    run_jobs('arab',i)
    run_jobs('NFBC',i)
