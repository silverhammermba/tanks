all: tanks

tanks: main.o
	g++ -o launch main.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -std=c++11

helpers.o: helpers.cpp
	g++ -c helpers.cpp -std=c++11

clean:
	rm -f *~ *.o *.out launch
