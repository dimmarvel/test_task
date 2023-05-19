all:
	g++ -g -Wall main.cpp config.cpp app.cpp parser.cpp threads.cpp -o out
	
run:
	./out