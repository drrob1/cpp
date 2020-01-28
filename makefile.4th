objects = hellomain.o hello.o
CXXFLAGS = -Wall -g -Os

hello : $(objects) 
	g++ $(CXXFLAGS)  -o hello $(objects)

hellomain.o :   # blank prerequisite to just use the implicit hellomain.cpp.  Yeah, it does work.

hello.o :       # blank prerequisit to see if it works to use just implciit rules here.

.PHONY : clean
clean : 
	-rm -v hello $(objects)

