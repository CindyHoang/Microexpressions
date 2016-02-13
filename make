CXX = clang++
VERSION = 
EXEC = sketch 
FLAGS = -Wall -Werror -Wextra
LDDFLAGS = -L/usr/local/opt/opencv3/lib
LIBS = -lopencv_core.3.0 -lopencv_imgproc.3.0 -lopencv_imgcodecs.3.0
CPPFLAGS = -I/usr/local/opt/opencv3/include
OBJS = sketch.o

build: $(OBJS)
	$(CXX) $(VERSION) $^ $(LDDFLAGS) $(LIBS) $(CPPFLAGS) $(FLAGS) -o $(EXEC) 

clean: 
	rm -f *.o *.out $(EXEC)