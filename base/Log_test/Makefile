.PHONY: clean
objects=LogFile_test.o LogFile.o Thread.o FileUtil.o CountDownLatch.o LogStream.o
test:$(objects)
	g++ -std=c++11 -o test $(objects) -lpthread
LogFile.o:../FileUtil.h ../mutex.h ../noncopyable.h
	g++ -std=c++11 -c ../LogFile.cpp 
LogFile_test.o: ../LogFile.h ../LogStream.h
	g++ -std=c++11 -c LogFile_test.cpp 
FileUtil.o:
	g++ -std=c++11 -c ../FileUtil.cpp
Thread.o:
	g++ -std=c++11 -c ../Thread.cpp -lpthread
CountDownLatch.o:../mutex.h ../Condition.h
	g++ -std=c++11 -c ../CountDownLatch.cpp -lpthread
LogStream.o:
	g++ -std=c++11 -c ../LogStream.cpp -lpthread
clean:
	rm *.o test