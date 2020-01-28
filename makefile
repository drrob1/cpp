# From a usenet post, the target of the prerequisite rule had the source file, not an object file.
# But then another post said it should be an object file.  It's interesting that it works anyway.
objects = tokenizec2.o getcommandline2.o timlibc2.o hpcalcc2.o holidaycalc2.o makesubst.o rpnc2.o
CXXFLAGS = -Wall -g  -std=c++11

rpnc2 : $(objects) 
	g++ $(CXXFLAGS) -o rpnc2 $(objects)

tokenizec2.o :   tokenizec2.h  macros.h tokenizec2.cpp
	g++ $(CXXFLAGS) -g -c tokenizec2.cpp

getcommandline2.o : macros.h getcommandline2.cpp
	g++ $(CXXFLAGS) -g -c getcommandline2.cpp

timlibc2.o : timlibc2.h macros.h timlibc2.cpp
	g++ $(CXXFLAGS) -g -c timlibc2.cpp

hpcalcc2.o : hpcalcc2.h macros.h hpcalcc2.cpp
	g++ $(CXXFLAGS) -g -c hpcalcc2.cpp

holidaycalc2.o : holidaycalc2.h macros.h holidaycalc2.cpp
	g++ $(CXXFLAGS) -g -c holidaycalc2.cpp


makesubst.o	: macros.h makesubst.h makesubst.cpp
	g++ $(CXXFLAGS) -g -c makesubst.cpp

rpnc2.o : tokenizec2.h macros.h getcommandline2.h timlibc2.h hpcalcc2.h holidaycalc2.h rpnc2.cpp
	g++ $(CXXFLAGS) -g -c rpnc2.cpp

.PHONY : clean
clean : 
	-rm -v rpnc2 $(objects)

