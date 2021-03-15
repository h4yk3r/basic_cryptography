AES : AES.o aesutil.o
	g++ -o AES AES.o aesutil.o

AES.o : AES.cpp aesutil.h
	g++ -c AES.cpp

aesutil.o : aesutil.cpp
	g++ -c aesutil.cpp

clean :
	rm AES AES.o aesutil.o
