CXX=g++
LD=${CXX}
CXXFLAGS+=-Wall -Wextra -pedantic -std=c++11 -O2 -pg
#CXXFLAGS+=-Wall -Wextra -pedantic -std=c++11 -O1 -g
LDFLAGS+=-lm $(CXXFLAGS)

OBJS=main.o ConfigFileParser.o Particle.o InitialConditions.o Cell.o OctoTree_Serial.o  Writter.o

all: barnes_hut_seq

barnes_hut_seq: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f  barnes_hut_seq *.o *~
