all: tanks

tanks: main.o helpers.o tank.o projectile.o
	g++ -o launch main.o helpers.o tank.o projectile.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -std=c++11

helpers.o: helpers.cpp helpers.hpp
	g++ -c helpers.cpp -std=c++11

tank.o: tank.cpp tank.hpp
	g++ -c tank.cpp -std=c++11

projectile.o: projectile.cpp projectile.hpp
	g++ -c projectile.cpp -std=c++11

clean:
	rm -f *~ *.o *.out launch
