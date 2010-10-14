all: example

example: example.o
	g++ -framework gecode -framework libxml -framework QtCore -framework QtGui -o "example"  ./example.o -lIscore -lDeviceManager

example.o: example.cpp
	g++ -O3 -c -fmessage-length=0 -o"example.o" "example.cpp"

clean:
	rm -rf example.o example