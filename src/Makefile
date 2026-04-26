LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)
CXXFLAGS =  -std=c++20
GTKFLAGS = `pkg-config --cflags --libs gtk4`
OLD = $(shell find ./ -name "*.o")

SOURCE := callbacks/callbacks.cpp config/config.cpp simple_canvas.cpp controller/controller.cpp main.cpp

%.o: %.c
	g++ $(CXXFLAGS) -c $< -o $@

all: clean 3Dtransformer.a
	g++ $(CXXFLAGS) $(SOURCE) -o game.out $(GTKFLAGS) -lm -L. -l3Dtransformer
# 	valgrind --leak-check=full --track-origins=yes -s ./game.out
	./game.out

install:

uninstall:

clean:
	rm -rf *.a $(OLD) game.out

dvi:

dist:
 
tests:

3Dtransformer.a: $(LIB_OBJS)
	ar rcs lib3Dtransformer.a $^
