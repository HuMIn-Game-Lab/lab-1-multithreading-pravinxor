compile: ./Code/*.cpp
	clang++ -pipe -Wall -std=c++17 ./Code/main.cpp ./Code/system/*.cpp ./Code/jobs/*.cpp -o system

demo: ./Code/Demo/main.cpp
	clang++ -pipe -Wall ./Code/Demo/*.cpp -o demo
