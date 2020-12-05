CFLAGS = -Wall -lpthread -std=c++17 -g

kliens: ./cliens/*.cpp ./common/*.cpp ./common/*.h
	g++ -o kliens ./cliens/*.cpp ./common/*.cpp $(CFLAGS)

szerver: ./serv/*.cpp ./common/*.cpp ./common/*.h ./serv/servH.h
	g++ -o szerver ./serv/*.cpp ./common/*.cpp $(CFLAGS)

all: kliens szerver

clean: 
	rm -f kliens
	rm -f szerver