all :		UDPserver.exe UDPclient.exe

UDPserver.exe : UDPserver.o
		gcc -o UDPserver.exe UDPserver.o

UDPclient.exe :	UDPclient.o
		gcc -o UDPclient.exe UDPclient.o

UDPserver.o :	UDPserver.c  myfunction.h
		gcc -c UDPserver.c

UDPclient.o :	UDPclient.c  myfunction.h
		gcc -c UDPclient.c

clean:
		-rm	*.exe *.o
