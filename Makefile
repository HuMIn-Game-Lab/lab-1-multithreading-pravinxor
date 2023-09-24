compile: 
	clang++ -pipe -Wall -std=c++17 ./Code/*.cpp -o output

demo: ./Data/Demo/main.cpp
	clang++ -pipe -Wall ./Data/Demo/*.cpp -o demo
