dir_include = ./include
dir_src = ./src
dir_obj = ./obj

c = g++
c_opt = -Wall -Wpedantic -Werror -I$(dir_include)
l_opt = 

out = main
obj = $(addprefix $(dir_obj)/, main.o storage.o)

main_src = $(addprefix $(dir_src)/, main.cpp)
main_h = $(addprefix $(dir_include)/, storage.hpp)
storage_src = $(addprefix $(dir_src)/, storage.cpp)
storage_h = $(addprefix $(dir_include)/, storage.hpp)

.PHONY: all run clean distclean rebuild

all: $(out)

run: all
	./main

clean:
	@rm -rfv $(dir_obj)/*

distclean: clean
	@rm -rfv $(out)

rebuild: distclean
	@make all

$(out): $(obj)
	$(c) $(l_opt) -o$@ $^

.SECONDEXPANSION:

$(obj): $(dir_obj)/%.o: $$(%_src) $$(%_h)
	$(c) -c $(c_opt) -o$@ $(^:%.hpp=)
