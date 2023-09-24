compile: 
	clang++ -pipe -Wall -g -std=c++17 ./Code/*.cpp -o output

demo: ./Data/Demo/main.cpp
	clang++ ./Data/Demo/*.cpp -o demo
