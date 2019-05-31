.PHONY: clean
objects1=ChatClient.o InetAddress.o Socket.o Buffer.o Connector.o \
	LogFile.o Thread.o FileUtil.o CountDownLatch.o LogStream.o \
		AsyncLogging.o Logging.o Channel.o EventLoop.o \
		Poller.o PollPoller.o TimeQueue.o Timer.o TcpClient.o TcpConnection.o EventLoopThread.o Epollpoller.o

ChatClient:$(objects1)
	g++ -std=c++11 -o ChatClient $(objects1) -lpthread -g
ChatClient.o:
	g++ -std=c++11 -c ChatClient.cpp -g
InetAddress.o:
	g++ -std=c++11 -c ../net/InetAddress.cpp -g
Socket.o:
	g++ -std=c++11 -c ../net/Socket.cpp -g
Acceptor.o:
	g++ -std=c++11 -c ../Acceptor.cpp -g
Buffer.o:
	g++ -std=c++11 -c ../Buffer.cpp -g
Connector.o:
	g++ -std=c++11 -c ../Connector.cpp -g
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
TcpClient.o:
	g++ -std=c++11 -c ../TcpClient.cpp -lpthread -g
TcpConnection.o:
	g++ -std=c++11 -c ../TcpConnection.cpp -g
EventLoopThread.o:
	g++ -std=c++11 -c ../EventLoopThread.cpp -g
Epollpoller.o:
	g++ -std=c++11 -c ../Epollpoller.cpp -g
clean:
	rm *.o ChatClient WebServer_Log_File