CXX=mpic++
LD=${CXX}
CXXFLAGS+=-Wall -Wextra -pedantic -std=c++11 -O2
#CXXFLAGS+=-Wall -Wextra -pedantic -std=c++11 -O1 -g
LDFLAGS+=-lm  $(CXXFLAGS)

OBJS=main.o ConfigFileParser.o Particle.o InitialConditions.o Cell.o  OctoTree_Parallel.o Writter.o

all: barnes_hut_par

barnes_hut_par: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f  barnes_hut_par *.o *~
