.PHONY: clean
objects=LogFile_test.o LogFile.o Thread.o FileUtil.o CountDownLatch.o LogStream.o \
		AsyncLogging.o
test:$(objects)
	g++ -std=c++11 -o test $(objects) -lpthread -g
LogFile.o:../FileUtil.h ../mutex.h ../noncopyable.h
	g++ -std=c++11 -c ../LogFile.cpp -g
LogFile_test.o: ../LogFile.h ../LogStream.h
	g++ -std=c++11 -c LogFile_test.cpp -g
FileUtil.o:
	g++ -std=c++11 -c ../FileUtil.cpp -g
Thread.o:
	g++ -std=c++11 -c ../Thread.cpp -lpthread -g
CountDownLatch.o:../mutex.h ../Condition.h
	g++ -std=c++11 -c ../CountDownLatch.cpp -lpthread -g
LogStream.o:
	g++ -std=c++11 -c ../LogStream.cpp -lpthread -g
AsyncLogging.o:
	g++ -std=c++11 -c ../AsyncLogging.cpp -lpthread -g

clean:
	rm *.o test