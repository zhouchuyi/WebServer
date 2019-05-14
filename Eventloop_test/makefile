.PHONY: clean
objects=eventloop_test.o LogFile.o Thread.o FileUtil.o CountDownLatch.o LogStream.o \
		AsyncLogging.o Logging.o Channel.o EventLoop.o \
		Poller.o PollPoller.o TimeQueue.o Timer.o
test:$(objects)
	g++ -std=c++11 -o test $(objects) -lpthread -g
LogFile.o:../base/FileUtil.h ../base/mutex.h ../base/noncopyable.h
	g++ -std=c++11 -c ../base/LogFile.cpp -g
FileUtil.o:
	g++ -std=c++11 -c ../base/FileUtil.cpp -g
Thread.o:
	g++ -std=c++11 -c ../base/Thread.cpp -lpthread -g
CountDownLatch.o:../base/mutex.h ../base/Condition.h
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
eventloop_test.o:
	g++ -std=c++11 -c ./eventloop_test.cpp -lpthread -g
clean:
	rm *.o test WebServer_Log_File