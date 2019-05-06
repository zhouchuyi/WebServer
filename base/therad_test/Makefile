.PHONY: clean
objects=thread_test.o CountDownLatch.o Thread.o
test : $(objects)
	g++ -std=c++11 -o test $(objects) -lpthread
CountDownLatch.o:../mutex.h ../Condition.h
	g++ -std=c++11 -c ../CountDownLatch.cpp -lpthread
Thread.o:../CountDownLatch.h
	g++ -std=c++11 -c ../Thread.cpp -lpthread
thread_test.o:../mutex.h ../Thread.h
	g++ -std=c++11 -c thread_test.cpp -lpthread
clean:
	rm test $(objects)
