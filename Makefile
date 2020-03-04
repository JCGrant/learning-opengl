run:
	mkdir -p bin
	clang++ -Wall -std=c++11 -stdlib=libc++ -lglfw -Iinclude lib/* src/* -o bin/main
	./bin/main
