LIB_SOURCE := $(shell find include/ -name "*.cpp")
LIB_OBJS := $(LIB_SOURCE:.cpp=.o)

%.o: %.c
	g++ -c $< -o $@

all: clean 3Dtransformer.a
	g++ simple_canvas.cpp main.cpp -o game
	./game


install:

uninstall:

clean:
	rm -rf *.o *.a

dvi:

dist:
 
tests:

3Dtransformer.a: $(LIB_OBJS)
	ar rcs 3Dtransformer.a $^
