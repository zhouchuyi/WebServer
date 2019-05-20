.PHONY: clean
objects1=client.o InetAddress.o Socket.o
objects2=server.o InetAddress.o Socket.o

client:$(objects1)
	g++ -std=c++11 -o client $(objects1) -g
server:$(objects2)
	g++ -std=c++11 -o server $(objects2) -g
client.o:
	g++ -std=c++11 -c client.cc -g
server.o:
	g++ -std=c++11 -c server.cc -g
InetAddress.o:
	g++ -std=c++11 -c ../net/InetAddress.cpp -g
Socket.o:
	g++ -std=c++11 -c ../net/Socket.cpp -g


clean:
	rm *.o server client 