.PHONY: clean
objects=test.o CountDownLatch.o Thread.o ThreadPoll.o 
test : $(objects)
	g++ -std=c++11 -o test $(objects) -lpthread -g
CountDownLatch.o:../mutex.h ../Condition.h
	g++ -std=c++11 -c ../CountDownLatch.cpp -lpthread -g
Thread.o:../CountDownLatch.h
	g++ -std=c++11 -c ../Thread.cpp -lpthread -g
test.o:../mutex.h ../Thread.h
	g++ -std=c++11 -c test.cpp -lpthread -g
ThreadPoll.o:../ThreadPoll.h
	g++ -std=c++11 -c ../ThreadPoll.cpp -lpthread -g
clean:
	rm test $(objects)
