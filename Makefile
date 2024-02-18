debug: main.o player.o
	clang++ --debug -O0 main.o player.o -lraylib -lopengl32 -lgdi32 -lwinmm -o out

main.o: src/main.cpp
	g++ -c src/main.cpp

player.o: src/player.cpp
	g++ -c src/player.cpp

clean:
	del *.o
	del out.exe
