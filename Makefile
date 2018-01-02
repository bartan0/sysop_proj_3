dir_include = ./include
dir_src = ./src
dir_obj = ./obj

run_cmd = ./main 4 3 8

c = g++
c_opt = -std=c++11 -Wall -Wpedantic -Werror -I$(dir_include)
l_opt = -pthread

out = main
obj = $(addprefix $(dir_obj)/, \
	main.o \
	storage.o \
	storage_item.o \
)

main_src = $(addprefix $(dir_src)/, \
	main.cpp \
)
main_h = $(addprefix $(dir_include)/, \
	config.hpp \
	storage.hpp \
	storage_item.hpp \
)
storage_src = $(addprefix $(dir_src)/, \
	storage.cpp \
)
storage_h = $(addprefix $(dir_include)/, \
	storage.hpp \
)
storage_item_src = $(addprefix $(dir_src)/, \
	storage_item.cpp \
)
storage_item_h = $(addprefix $(dir_include)/, \
	storage.hpp \
	storage_item.hpp \
)

.PHONY: all run clean distclean rebuild

all: $(out)

run: all
	$(run_cmd)

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
	$(c) -c $(c_opt) -o$@ $(filter %.cpp, $^)
