objects=Thread_test.o CountDownLatch.o Thread.o
test : $(objects)
	clang++ -std=c++11 -o test $(objects)
CountDownLatch.o:../mutex.h ../Condition.h
	clang++ -std=c++11 -c ../CountDownLatch.cpp
Thread.o:../CountDownLatch.h
	clang++ -std=c++11 -c ../Thread.cpp
Thread_test.o:../mutex.h ../Thread.h
	clang++ -std=c++11 -c Thread_test.cpp
