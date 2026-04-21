LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)
CXXFLAGS =  -std=c++20
GTKFLAGS = `pkg-config --cflags --libs gtk4`
OLD = $(shell find ./ -name "*.o")

%.o: %.c
	g++ $(CXXFLAGS) -c $< -o $@

all: clean 3Dtransformer.a
	g++ $(CXXFLAGS) simple_canvas.cpp main.cpp -o game $(GTKFLAGS) -lm -L. -l3Dtransformer
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
