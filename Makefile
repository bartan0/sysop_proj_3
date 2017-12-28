dir_include = ./include/
dir_src = ./src/
dir_obj = ./obj/

c = g++
c_opt = -std=c++11 -Wall -Werror -Wpedantic -I$(dir_include) -DDEBUG
l_opt =

all: x

x: $(dir_obj)main.o $(dir_obj)storage.o $(dir_obj)debug.o
	$(c) $(l_opt) -ox $(dir_obj)storage.o $(dir_obj)debug.o $(dir_obj)main.o

$(dir_obj)main.o: $(dir_src)main.cpp $(dir_include)storage.hpp \
$(dir_include)debug.hpp
	$(c) -c $(c_opt) -o$(dir_obj)main.o $(dir_src)main.cpp

$(dir_obj)storage.o: $(dir_src)storage.cpp $(dir_include)storage.hpp
	$(c) -c $(c_opt) -o$(dir_obj)storage.o $(dir_src)storage.cpp

$(dir_obj)debug.o: $(dir_src)debug.cpp $(dir_include)debug.hpp
	$(c) -c $(c_opt) -o$(dir_obj)debug.o $(dir_src)debug.cpp

clean:
	rm -rfv obj/*

distclean: clean
	rm -rfv x

rebuild: distclean all

run: all
	./x
