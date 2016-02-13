CXX = g++
SOURCE = micro.cpp
VERSION = 
EXEC = sketch 
FLAGS = -Wall -Werror -Wextra
LDDFLAGS = -L/usr/lib
LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui
CPPFLAGS = -I/usr/include
OBJS = sketch.o

build:
	$(CXX) $(SOURCE) $(VERSION) $^ $(LDDFLAGS) $(LIBS) $(CPPFLAGS) $(FLAGS) -o $(EXEC) 

clean: 
	rm -f *.o *.out $(EXEC)