all :		UDPserver.exe UDPclient.exe

UDPserver.exe : UDPserver.o
		gcc -o UDPserver.exe UDPserver.o -pthread

UDPclient.exe :	UDPclient.o
		gcc -o UDPclient.exe UDPclient.o -pthread

UDPserver.o :	UDPserver.c  myfunction.h
		gcc -c UDPserver.c

UDPclient.o :	UDPclient.c  myfunction.h
		gcc -c UDPclient.c

clean:
		-rm	*.exe *.o
