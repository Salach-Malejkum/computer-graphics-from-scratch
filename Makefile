all:
	g++ -o main main.cpp -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2

run:
	./main