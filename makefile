DIR = Code/

COMP = g++ -Wall -c -DUSINGGCC
LINK = g++ -Wall -o

all: COMP += -O2
all: crowdsim

debug: COMP += -DDEBUG -g
debug: crowdsim

crowdsim: 