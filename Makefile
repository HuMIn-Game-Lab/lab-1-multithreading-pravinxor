compile: 
	clang++ -pipe -Wall -g -std=c++17 ./Code/*.cpp -o output

demo: ./Data/Demo/main.cpp
	clang++ ./Data/Demo/main.cpp -o demo
