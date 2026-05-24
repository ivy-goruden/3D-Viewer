LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)
CXXFLAGS =  -std=c++20
GTKFLAGS = `pkg-config --cflags --libs gtk4`
OLD = $(shell find ./ -name "*.o")
TESTS := $(shell find tests/ -name "*.cpp")
TESTS_OBJS := $(TESTS:.cpp=.o)
SOURCE := callbacks/callbacks.cpp config/config.cpp simple_canvas.cpp controller/controller.cpp main.cpp
GUI_SOURCE := $(shell find gui/ -name "*.cpp")
GTEST_LIBS = -lgtest -lgtest_main -pthread

.PHONY: clean

%.o: %.c
	g++ $(CXXFLAGS) -c $< -o $@

all: gtk02run

install:

uninstall:

dvi:

dist:
 
cf-check:
	clang-format -i ./**/*.hpp
	clang-format -i ./**/*.cpp
	clang-format -i ./**/*.[hc]
	clang-format -i ./*.cpp

clean:
	find . -type f \( -name '*.app' -o -name '*.out' -o -name '*.so' -o -name '*.a' -o -name '*.o' \) -exec rm -f {} +

tests: $(TESTS_OBJS) 3Dtransformer.a
	g++ $(TESTS_OBJS) -L. -l3Dtransformer -o tests.out $(GTEST_LIBS)
	./tests.out

3Dtransformer.a: $(LIB_OBJS)
	ar rcs lib3Dtransformer.a $^

gtk02: 3Dtransformer.a
	g++ -g -O0 $(CXXFLAGS) gtk02.cpp $(GUI_SOURCE) controller/controller.cpp -I . -L. -l3Dtransformer -o gtk02.app $(GTKFLAGS) -lm

gtk02run: gtk02
	./gtk02.app

valgrind:
	valgrind  --suppressions=/usr/share/glib-2.0/valgrind/glib.supp \
  --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp --suppressions=my.supp --leak-check=full --show-leak-kinds=definite,indirect --track-origins=yes -s ./gtk02.app
# 	valgrind --leak-check=full ./gtk02.app 2>&1 | grep -E "definitely.*gtk02.app|lost.*gtk02.app" || true
