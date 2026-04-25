LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)
CXXFLAGS =  -std=c++20
GTKFLAGS = `pkg-config --cflags --libs gtk4`
OLD = $(shell find ./ -name "*.o")

%.o: %.c
	g++ $(CXXFLAGS) -c $< -o $@

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

gtk01:
	g++ $(CXXFLAGS) gtk01.cpp -o gtk01.app $(GTKFLAGS) -lm

gtk01run: gtk01
	./gtk01.app

gtk02:
	g++ $(CXXFLAGS) gtk02.cpp gui/gui_app.cpp -o gtk02.app $(GTKFLAGS) -lm

gtk02run: gtk02
	./gtk02.app
