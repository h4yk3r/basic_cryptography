rsamain : rsamain.o rsa.o fileutil.o
	g++ -o rsamain rsamain.o rsa.o fileutil.o -std=c++11 -lgmp -lgmpxx

rsamain.o : rsamain.cpp rsa.h fileutil.h
	g++ -c -g rsamain.cpp -std=c++11 -lgmp -lgmpxx

rsa.o : rsa.cpp
	g++ -c -g rsa.cpp -std=c++11 -lgmp -lgmpxx

fileutil.o : fileutil.cpp
	g++ -c -g fileutil.cpp -std=c++11

clean :
	rm rsamain.o rsa.o fileutil.o
