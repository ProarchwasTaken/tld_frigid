dependencies = main.o levels.o player.o goal_door.o solid_tile.o ice_spike.o
libraries = -lraylib -lopengl32 -lgdi32 -lwinmm

debug: $(dependencies)
	clang++ --debug -O0 $(dependencies) $(libraries) -o out

final: $(dependencies)
	g++ -O3 -mwindows $(dependencies) $(libraries) -o game

main.o: src/main.cpp
	g++ -c src/main.cpp

levels.o: src/levels.cpp
	g++ -c src/levels.cpp

player.o: src/player.cpp
	g++ -c src/player.cpp

goal_door.o: src/goal_door.cpp
	g++ -c src/goal_door.cpp

solid_tile.o: src/solid_tile.cpp
	g++ -c src/solid_tile.cpp

ice_spike.o: src/ice_spike.cpp
	g++ -c src/ice_spike.cpp

clean:
	del *.o
	del out.exe
