all : main_direct.x

TH ?= 1
N ?= 1000
EPS ?= 0.1
NDIM ?= 3

#state_final_1000.dat : main_direct.x
#		./main_direct.x $(N) $(EPS) $(TH) $(NDIM)

main_direct.x : main_direct.c t_evol.c initial.c
		g++ -O1 -g -pg main_direct.c initial.c t_evol.c -lm  -o main_direct.x
		#gcc main_direct.c initial.c t_evol.c -lm -fopenmp -o main_direct.x #Uncomment for OpenMP

#run :
#		./main_direct.x $(N) $(EPS) $(TH) $(NDIM)

clean :
	rm state_*
	rm ./*x
