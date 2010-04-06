all: example

example: example.o
	g++ -framework gecode -framework libxml -o "example"  ./example.o -lIscore

example.o: example.cpp
	g++ -O3 -Wall -c -fmessage-length=0 -o"example.o" "example.cpp"

clean:
	rm -rf example.o example