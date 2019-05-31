.PHONY: clean
objects=ChatServer.o LogFile.o Thread.o FileUtil.o CountDownLatch.o LogStream.o \
		AsyncLogging.o Logging.o Channel.o EventLoop.o \
		Poller.o PollPoller.o TimeQueue.o Timer.o EventLoopThread.o EventLoopThreadPool.o \
		TcpServer.o InetAddress.o Socket.o Acceptor.o TcpConnection.o Buffer.o Epollpoller.o
test:$(objects)
	g++ -std=c++11 -o ChatServer $(objects) -lpthread -g
ChatServer.o:
	g++ -std=c++11 -c ChatServer.cpp -g
LogFile.o:
	g++ -std=c++11 -c ../base/LogFile.cpp -g
FileUtil.o:
	g++ -std=c++11 -c ../base/FileUtil.cpp -g
Thread.o:
	g++ -std=c++11 -c ../base/Thread.cpp -lpthread -g
CountDownLatch.o:
	g++ -std=c++11 -c ../base/CountDownLatch.cpp -lpthread -g
LogStream.o:
	g++ -std=c++11 -c ../base/LogStream.cpp -lpthread -g
AsyncLogging.o:
	g++ -std=c++11 -c ../base/AsyncLogging.cpp -lpthread -g
Logging.o:
	g++ -std=c++11 -c ../base/Logging.cpp -lpthread -g
Channel.o:
	g++ -std=c++11 -c ../Channel.cpp -lpthread -g
EventLoop.o:
	g++ -std=c++11 -c ../EventLoop.cpp -lpthread -g
Poller.o:
	g++ -std=c++11 -c ../Poller.cpp -lpthread -g
PollPoller.o:
	g++ -std=c++11 -c ../PollPoller.cpp -lpthread -g
TimeQueue.o:
	g++ -std=c++11 -c ../TimeQueue.cpp -lpthread -g
Timer.o:
	g++ -std=c++11 -c ../Timer.cpp -lpthread -g
EventLoopThread.o:
	g++ -std=c++11 -c ../EventLoopThread.cpp -lpthread -g
EventLoopThreadPool.o:
	g++ -std=c++11 -c ../EventLoopThreadPool.cpp -lpthread -g
TcpServer.o:
	g++ -std=c++11 -c ../TcpServer.cpp -lpthread -g
server.o:
	g++ -std=c++11 -c ./server.cpp -lpthread -g
InetAddress.o:
	g++ -std=c++11 -c ../net/InetAddress.cpp -g
Socket.o:
	g++ -std=c++11 -c ../net/Socket.cpp -g
Acceptor.o:
	g++ -std=c++11 -c ../Acceptor.cpp -g
TcpConnection.o:
	g++ -std=c++11 -c ../TcpConnection.cpp -g
Buffer.o:
	g++ -std=c++11 -c ../Buffer.cpp -g

Epollpoller.o:
	g++ -std=c++11 -c ../Epollpoller.cpp -g

clean:
	rm *.o ChatServer WebServer_Log_File