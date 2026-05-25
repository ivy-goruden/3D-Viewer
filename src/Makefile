SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
INSTALLDIR = ../usr
TAR = tar
TARFLAGS = -czf
DISTNAME = 3dviewer
DISTDIR = ../dist
DOXYFILE = Doxyfile
DOCDIR = ../docs
TARGET = 3dviewer.app

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

all: $(TARGET)
	./$(TARGET)

# Создание папок
$(DISTDIR):
	mkdir -p $(DISTDIR)

# Установка программы
install: $(TARGET)
	install -d $(INSTALLDIR)
	install -d $(INSTALLDIR)/gui
	install -m 755 $(TARGET) $(INSTALLDIR)/$(TARGET)
	install -m 755 lib3Dtransformer.a $(INSTALLDIR)/lib3Dtransformer.a
	install -m 755 gui/3dviewer.ui $(INSTALLDIR)/gui/3dviewer.ui
	@echo "Программа установлена в $(INSTALLDIR)"

# Удаление установленной программы
uninstall:
	rm -fr $(INSTALLDIR)
	@echo "Программа удалена из $(INSTALLDIR)"

# Создание дистрибутива
dist: clean $(DISTDIR)
	mkdir -p $(DISTDIR)/$(DISTNAME)/src
	cp -r $(SRCDIR) $(DISTDIR)/$(DISTNAME)/src
	cp -r $(SRCDIR)/Makefile $(DISTDIR)/$(DISTNAME)/src
	cp -r $(SRCDIR)/Doxyfile $(DISTDIR)/$(DISTNAME)/src
	$(TAR) $(TARFLAGS) $(DISTDIR)/$(DISTNAME).tar.gz $(DISTDIR)/$(DISTNAME)

# Цель для генерации DVI документации через Doxygen
dvi: $(DOXYFILE)
	doxygen $(DOXYFILE)
	@if [ -d "$(DOCDIR)/latex" ]; then \
		cd $(DOCDIR)/latex && make && cd -; \
		echo "DVI документация сгенерирована в $(DOCDIR)/latex/refman.dvi"; \
	else \
		echo "Ошибка: LaTeX директория не найдена"; \
	fi

# Создание Doxyfile, если его нет
$(DOXYFILE):
	doxygen -g $(DOXYFILE)
	@echo "Создан шаблонный $(DOXYFILE), пожалуйста, настройте его"
 
cf-check:
	clang-format -i ./**/*.hpp
	clang-format -i ./**/*.cpp
	clang-format -i ./**/*.[hc]
	clang-format -i ./*.cpp

clean:
	find . -type f \( -name '*.app' -o -name '*.out' -o -name '*.so' -o -name '*.a' -o -name '*.o' \) -exec rm -f {} +

clean-doc:
	rm -rf $(DOCDIR)

tests: $(TESTS_OBJS) 3Dtransformer.a
	g++ $(TESTS_OBJS) -L. -l3Dtransformer -o tests.out $(GTEST_LIBS)
	./tests.out

3Dtransformer.a: $(LIB_OBJS)
	ar rcs lib3Dtransformer.a $^

$(TARGET): 3Dtransformer.a
	g++ $(CXXFLAGS) gtk02.cpp $(GUI_SOURCE) controller/controller.cpp -I . -L. -l3Dtransformer -o $(TARGET) $(GTKFLAGS) -lm

valgrind: clean $(TARGET)
	valgrind  --log-file=valgrind_report.txt \
	--suppressions=/usr/share/glib-2.0/valgrind/glib.supp \
  	--suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp \
  	--suppressions=my.supp \
	--leak-check=full --show-leak-kinds=definite,indirect --track-origins=yes -s ./$(TARGET)
# 	valgrind --leak-check=full ./gtk02.app 2>&1 | grep -E "definitely.*gtk02.app|lost.*gtk02.app" || true
