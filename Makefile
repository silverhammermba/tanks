all: tanks

tanks: main.o tank.o projectile.o tread.o turret.o helpers.o
	g++ -o launch main.o helpers.o tank.o projectile.o tread.o turret.o -lBox2D -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp helpers.o
	g++ -c main.cpp -std=c++11

helpers.o: helpers.cpp helpers.hpp
	g++ -c helpers.cpp -std=c++11

tank.o: tank.cpp tank.hpp helpers.o projectile.o tread.o turret.o
	g++ -c tank.cpp -std=c++11

projectile.o: projectile.cpp projectile.hpp helpers.o
	g++ -c projectile.cpp -std=c++11

tread.o: tread.cpp tread.hpp helpers.o
	g++ -c tread.cpp -std=c++11

turret.o: turret.cpp turret.hpp helpers.o
	g++ -c turret.cpp -std=c++11

clean:
	rm -f *~ *.o *.out launch
