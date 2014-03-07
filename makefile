DIR = Code/

COMP = g++ -Wall -c -DUSINGGCC
LINK = g++ -Wall -o

# Compiles with optimization
all: COMP += -O2
all: crowdsim

# Compiles with no optimization and debug mode enabled
debug: COMP += -DDEBUG -g
debug: crowdsim

crowdsim: 
