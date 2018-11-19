import numpy as np
import sys

if (len(sys.argv) != 2):
    sys.exit('Usage: python energia.py n_bodies')

n_bodies = 1000
n_bodies = int(sys.argv[1])

%cd '/home/shernand/Documents/EPFL/Semester-2/PHPC_SHDC/Project/N-Body_SDHC/Direct_Sum/'
%pwd

inicial = np.loadtxt('./state_inicial_{}.dat'.format(n_bodies))
final = np.loadtxt('./state_final_{}.dat'.format(n_bodies))

energia_k_i = np.sum(inicial[:,6])
energia_w_i = np.sum(inicial[:,7])

energia_k_f = np.sum(final[:,6])
energia_w_f = np.sum(final[:,7])

e_i = energia_k_i+energia_w_i
e_f = energia_k_f+energia_w_f

print('Energia inicial: {}'.format(e_i))
print('Energia final: {}'.format(e_f))
print('La energia se conserva un: {:0.2f}%'.format(100*(e_f-e_i)/e_i))
