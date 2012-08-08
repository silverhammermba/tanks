all: tanks

tanks: main.o tank.o projectile.o tread.o turret.o wall.o helpers.o 
	g++ -o launch main.o helpers.o tank.o projectile.o tread.o turret.o wall.o -lBox2D -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp tank.o projectile.o helpers.o wall.o
	g++ -c main.cpp -std=c++11

helpers.o: helpers.cpp helpers.hpp
	g++ -c helpers.cpp -std=c++11

tank.o: tank.cpp tank.hpp tread.o turret.o projectile.o entity.hpp helpers.o
	g++ -c tank.cpp -std=c++11

projectile.o: projectile.cpp projectile.hpp entity.hpp helpers.o
	g++ -c projectile.cpp -std=c++11

tread.o: tread.cpp tread.hpp entity.hpp helpers.o
	g++ -c tread.cpp -std=c++11

turret.o: turret.cpp turret.hpp entity.hpp helpers.o
	g++ -c turret.cpp -std=c++11

wall.o: wall.cpp wall.hpp entity.hpp helpers.o
	g++ -c wall.cpp -std=c++11

clean:
	rm -f *.o *.out launch
