all: 
	g++ ./src/*.cpp ./lib/*.cpp -o ./bin/release/project.o
debug:
	g++ -g ./src/*.cpp ./lib/*.cpp -o ./bin/debug/project.o



