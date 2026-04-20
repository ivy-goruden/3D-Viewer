LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)
CXXFLAGS =  -std=c++11
GTKFLAGS = `pkg-config --cflags --libs gtk4`
OLD = $(shell find ./ -name "*.o")

%.o: %.c
	g++ -c $< -o $@

all: clean 3Dtransformer.a
	g++ $(CXXFLAGS) simple_canvas.cpp controller/controller.cpp main.cpp -o game $(GTKFLAGS) -lm -L. -l3Dtransformer
# 	valgrind --leak-check=full --track-origins=yes -s ./game
	./game

install:

uninstall:

clean:
	rm -rf *.a $(OLD) game

dvi:

dist:
 
tests:

3Dtransformer.a: $(LIB_OBJS)
	ar rcs lib3Dtransformer.a $^
