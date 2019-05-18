.PHONY: clean
objects=buffer_test.o Buffer.o
test:$(objects)
	g++ -std=c++11 -o test $(objects) -g
Buffer.o:
	g++ -std=c++11 -c ../Buffer.cpp -g
buffer_test.o:
	g++ -std=c++11 -c ./buffer_test.cpp -g

clean:
	rm *.o test 