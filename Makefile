all: Dealer

Dealer:
	cd dealer/src/; g++ -g -O0 -fopenmp -Wall *.cpp -o Dealer; mv Dealer ../..
clean: 
	rm Dealer
