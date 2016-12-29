output: main.o sim.o readyQueue.o eventQueue.o
	g++ main.o sim.o readyQueue.o eventQueue.o -o sim

main.o: main.cpp
	g++ -Wall -ansi -pedantic  -c main.cpp

sim.o: sim.cpp sim.hpp
	g++ -Wall -ansi -pedantic -c sim.cpp sim.hpp

readyQueue.o: readyQueue.cpp readyQueue.hpp
	g++ -Wall -ansi -pedantic -c readyQueue.cpp readyQueue.hpp

eventQueue.o: eventQueue.cpp eventQueue.hpp
	g++ -Wall -ansi -pedantic -c eventQueue.cpp eventQueue.hpp

clean:
	rm *.o sim *.gch



