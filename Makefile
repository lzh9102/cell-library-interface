CXX=g++
CXXFLAGS=-g -O2 -Wall 
OBJS=library.o main.o
LDFLAGS= 

all: a.out

a.out: $(OBJS)
	$(CXX) -o a.out $(LDFLAGS) $(OBJS)

library.o: library.cpp library.h
	$(CXX) -o library.o -c $(CXXFLAGS) library.cpp

main.o: main.cpp library.h
	$(CXX) -o main.o -c $(CXXFLAGS) main.cpp

clean:
	rm -f library.o
	rm -f main.o
	rm -f a.out
