objects = ambiente.o tron.o main.o
project = mundo.bin
CXX = g++
CPPFLAGS = -Wall -ggdb

$(project) : $(objects)
	$(CXX) $(CPPFLAGS) $(objects) -o $(project)

ambiente.o: ambiente.h util.h tron.h
tron.o: tron.h util.h
main.o: tron.h util.h ambiente.h

.PHONY : clean
clean:
	-rm $(project) $(objects)
