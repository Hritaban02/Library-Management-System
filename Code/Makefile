# Makefile

# One single executable

a.out : main.o Book.o
	g++ main.o Book.o -o a.out
	
main.o : main.cpp 
	g++ -c -std=c++14 -W -Wall main.cpp
	
Book.o : Book.cpp Book.h
	g++ -c -std=c++14 -W -Wall Book.cpp
	
 




