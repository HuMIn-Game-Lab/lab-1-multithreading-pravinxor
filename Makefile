compile: 
	clang++ -pipe -Wall -g -std=c++17 ./Code/*.cpp -o output

demo: ./Data/Demo/main.cpp
	clang++ -pipe -Wall ./Data/Demo/*.cpp -o demo
