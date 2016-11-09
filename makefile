GPP = g++ -O3 -Wall -std=c++11
UTILS = ../Utilities
SCANNER = ../Utilities
SCANLINE = ../Utilities

M = main.o
C = configuration.o
SIM = simulation.o
PCT = onepct.o
VOTE = onevoter.o
R = myrandom.o
S = scanner.o
SL = scanline.o
U = utils.o

Aprog: $(M) $(C) $(SIM) $(PCT) $(VOTE) $(R) $(S) $(SL) $(U)
	$(GPP) -o Aprog $(M) $(C) $(SIM) $(PCT) $(VOTE) $(R) $(S) $(SL) $(U) $(TAIL)

main.o: main.h main.cc
	$(GPP) -o main.o -c main.cc

configuration.o: configuration.h configuration.cc
	$(GPP) -o configuration.o -c configuration.cc

simulation.o: simulation.h simulation.cc
	$(GPP) -o simulation.o -c simulation.cc

onepct.o: onepct.h onepct.cc
	$(GPP) -o onepct.o -c onepct.cc

onevoter.o: onevoter.h onevoter.cc
	$(GPP) -o onevoter.o -c onevoter.cc

myrandom.o: myrandom.h myrandom.cc
	$(GPP) -o myrandom.o -c myrandom.cc

scanner.o: $(SCANNER)/scanner.h $(SCANNER)/scanner.cc
	$(GPP) -o scanner.o -c $(SCANNER)/scanner.cc

scanline.o: $(SCANNER)/scanline.h $(SCANNER)/scanline.cc
	$(GPP) -o scanline.o -c $(SCANNER)/scanline.cc

utils.o: $(UTILS)/utils.h $(UTILS)/utils.cc
	$(GPP) -o utils.o -c $(UTILS)/utils.cc

clean:
	rm Aprog
	clean

