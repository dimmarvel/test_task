all:
	g++ -g main.cpp config.cpp app.cpp parser.cpp threads.cpp -o out
	
run:
	./out