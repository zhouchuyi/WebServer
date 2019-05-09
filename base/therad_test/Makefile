.PHONY: clean
objects=thread_test.o CountDownLatch.o Thread.o
test : $(objects)
	g++ -std=c++11 -o test $(objects) -lpthread -g
CountDownLatch.o:../mutex.h ../Condition.h
	g++ -std=c++11 -c ../CountDownLatch.cpp -lpthread -g
Thread.o:../CountDownLatch.h
	g++ -std=c++11 -c ../Thread.cpp -lpthread -g
thread_test.o:../mutex.h ../Thread.h
	g++ -std=c++11 -c thread_test.cpp -lpthread -g
clean:
	rm test $(objects)
