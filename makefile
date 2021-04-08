TestProb: optim.o
	g++ optim.o -o optim 

optim.o: optim.cpp
	g++ -c optim.cpp