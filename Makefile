all: Dealer

Dealer:
	cd dealer/src/; g++ -g -O0 -fopenmp -Wall *.cpp -o Dealer -lcrypto; mv Dealer ../..
clean: 
	rm Dealer
