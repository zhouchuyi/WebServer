.PHONY: clean
objects1=client.o InetAddress.o Socket.o Buffer.o
# objects2=server.o InetAddress.o Socket.o Acceptor.o \
# 	Channel.o EventLoop.o Poller.o PollPoller.o TimeQueue.o Timer.o Thread.o CountDownLatch.o

client:$(objects1)
	g++ -std=c++11 -o client $(objects1) -g
# server:$(objects2)
# 	g++ -std=c++11 -o server $(objects2) -g
client.o:
	g++ -std=c++11 -c client.cc -g
# server.o:
# 	g++ -std=c++11 -c server.cc -g
InetAddress.o:
	g++ -std=c++11 -c ../net/InetAddress.cpp -g
Socket.o:
	g++ -std=c++11 -c ../net/Socket.cpp -g
Acceptor.o:
	g++ -std=c++11 -c ../Acceptor.cpp -g
Buffer.o:
	g++ -std=c++11 -c ../Buffer.cpp -g
	
# Channel.o:
# 	g++ -std=c++11 -c ../Channel.cpp -lpthread -g
# EventLoop.o:
# 	g++ -std=c++11 -c ../EventLoop.cpp -lpthread -g
# Poller.o:
# 	g++ -std=c++11 -c ../Poller.cpp -lpthread -g
# PollPoller.o:
# 	g++ -std=c++11 -c ../PollPoller.cpp -lpthread -g
# TimeQueue.o:
# 	g++ -std=c++11 -c ../TimeQueue.cpp -lpthread -g
# Timer.o:
# 	g++ -std=c++11 -c ../Timer.cpp -lpthread -g
# Thread.o:
# 	g++ -std=c++11 -c ../base/Thread.cpp -lpthread -g
# CountDownLatch.o:../base/mutex.h ../base/Condition.h
# 	g++ -std=c++11 -c ../base/CountDownLatch.cpp -lpthread -g
clean:
	rm *.o client