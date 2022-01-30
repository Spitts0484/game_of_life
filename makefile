
all: main.o board.o
	g++ -g build/main.o build/board.o -lncurses -o game_of_life.exe
	./game_of_life.exe
	#gdb game_of_life.exe

main.o: src/main.cpp src/board.h
	g++ -c -g src/main.cpp -o build/main.o


board.o: src/board.cpp src/board.h
	g++ -c -g src/board.cpp -o build/board.o
