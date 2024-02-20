main: src/main.cpp
	g++ src/main.cpp -O2 -Wall -Wextra -o main

run:
	make main
	./main logs/trace1

tests:
	make main
	./main logs/trace1
	./main logs/trace2
	./main logs/trace3
	./main logs/trace4


dbg: src/main.cpp
	g++ src/main.cpp -g -Wall -Wextra -o main
