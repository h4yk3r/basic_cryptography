pss : pss.o rsautils.o
	g++ -o pss pss.o rsautils.o -std=c++11 -lgmp -lgmpxx -lssl -lcrypto

pss.o : pss.cpp rsa.h rsautils.h
	g++ -c -g pss.cpp -std=c++11 -lgmp -lgmpxx -lssl -lcrypto

rsautils.o : rsautils.cpp
	g++ -c -g rsautils.cpp -std=c++11 -lgmp -lgmpxx -lssl -lcrypto

clean :
	rm pss.o rsautils.o
