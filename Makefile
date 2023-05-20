all:
	g++ -g -Wall main.cpp config.cpp app.cpp parser.cpp threads.cpp -o out

build-tests:
	g++ -g -Wall config.cpp app.cpp parser.cpp threads.cpp tests.cpp -o tests