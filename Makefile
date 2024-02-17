debug: main.o
	clang++ --debug -O0 -Wall main.o -lraylib -lopengl32 -lgdi32 -lwinmm -o out

main.o: src/main.cpp
	g++ -c src/main.cpp

clean:
	del *.o
