all: tanks

tanks: main.o chassis.o factory.o tank.o projectile.o tread.o turret.o wall.o particle.o engine.o
	g++ -o launch main.o chassis.o factory.o engine.o tank.o projectile.o tread.o turret.o wall.o particle.o -lyaml-cpp -lBox2D -lsfml-graphics -lsfml-window -lsfml-system

engine.o: engine.cpp engine.hpp entity.hpp tread.hpp chassis.hpp projectile.hpp turret.hpp factory.hpp tank.hpp particle.hpp wall.hpp
	g++ -c engine.cpp -std=c++11

main.o: engine.hpp
	g++ -c main.cpp -std=c++11

factory.o: factory.cpp engine.hpp
	g++ -c factory.cpp -std=c++11

tank.o: tank.cpp engine.hpp
	g++ -c tank.cpp -std=c++11

projectile.o: projectile.cpp engine.hpp
	g++ -c projectile.cpp -std=c++11

tread.o: tread.cpp engine.hpp
	g++ -c tread.cpp -std=c++11

chassis.o: chassis.cpp engine.hpp
	g++ -c chassis.cpp -std=c++11

turret.o: turret.cpp engine.hpp
	g++ -c turret.cpp -std=c++11

wall.o: wall.cpp engine.hpp
	g++ -c wall.cpp -std=c++11

particle.o: particle.cpp engine.hpp
	g++ -c particle.cpp -std=c++11

clean:
	rm -f *.o *.out launch
