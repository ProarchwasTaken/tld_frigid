dependencies = main.o levels.o player.o goal_door.o 
libraries = -lraylib -lopengl32 -lgdi32 -lwinmm

debug: $(dependencies)
	clang++ --debug -O0 $(dependencies) $(libraries) -o out

main.o: src/main.cpp
	g++ -c src/main.cpp

levels.o: src/levels.cpp
	g++ -c src/levels.cpp

player.o: src/player.cpp
	g++ -c src/player.cpp

goal_door.o: src/goal_door.cpp
	g++ -c src/goal_door.cpp

clean:
	del *.o
	del out.exe
