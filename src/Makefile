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

gtk01:
	g++ $(CXXFLAGS) gtk01.cpp -o gtk01.app $(GTKFLAGS) -lm

gtk01run: gtk01
	./gtk01.app

gtk02: 3Dtransformer.a
	g++ $(CXXFLAGS) gtk02.cpp gui/gui_app.cpp gui/open_dialog.cpp simple_canvas.cpp controller/controller.cpp -L. -l3Dtransformer -o gtk02.app $(GTKFLAGS) -lm

gtk02run: gtk02
	./gtk02.app
