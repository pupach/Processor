OBJECTS = $(DIR_O)/Processor.o $(DIR_O)/virtual_machine.o $(DIR_O)/assembler.o $(DIR_O)/dis_assembler.o $(DIR_O)/command.o $(DIR_O)/stack.o $(DIR_O)/hash.o $(DIR_O)/work_with_file.o  $(DIR_O)/verification_and_check.o $(DIR_O)/str_func.o $(DIR_O)/str_input.o $(DIR_O)/onegin.o $(DIR_O)/func_sort.o $(DIR_O)/str_output.o $(DIR_O)/len_array.o $(DIR_O)/algoritm_sort.o

CC = g++
CC = g++
CFLAGS = -c -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
LFLAGS = -fsanitize=address
DIR_O = ./object
DIR_SOUR = .
DIR_STACK = ../Stack
DIR_ONEGIN_SOUR= ../str_project/source
DIR_ONEGIN_O = ../str_project/object
DIR_LIB = ../my_lib


# 1) use wildcard to get all cpps
# 2) add a rule to make .o from .cpp
# 3) add var for list of all .o or .cpp files needed to make a project
# 4) target <executable>: $(OBJFILES)
#	g++ $(OBJFILES) -o <
# 5)

kotik.elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o kotik.elf

$(DIR_O)/Processor.o: $(DIR_SOUR)/Processor.cpp
	$(CC) $(CFLAGS) $< -o $@


$(DIR_O)/virtual_machine.o: $(DIR_SOUR)/virtual_machine.cpp
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/dis_assembler.o: $(DIR_SOUR)/dis_assembler.cpp
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/assembler.o: $(DIR_SOUR)/assembler.cpp
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/command.o: $(DIR_SOUR)/command.cpp
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/stack.o: $(DIR_STACK)/stack.cpp $(DIR_STACK)/stack.h $(DIR_STACK)/hash.h $(DIR_LIB)/work_with_file.h $(DIR_STACK)/verification_and_check.h $(DIR_STACK)/type_stck_element.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/hash.o: $(DIR_STACK)/hash.cpp $(DIR_STACK)/hash.h $(DIR_STACK)/type_stck_element.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/work_with_file.o: $(DIR_LIB)/work_with_file.cpp $(DIR_LIB)/work_with_file.h $(DIR_STACK)/type_stck_element.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/verification_and_check.o: $(DIR_STACK)/verification_and_check.cpp $(DIR_STACK)/verification_and_check.h $(DIR_STACK)/stack.h $(DIR_STACK)/type_stck_element.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/str_func.o: $(DIR_LIB)/str_func.cpp $(DIR_LIB)/str_func.h $(DIR_ONEGIN_SOUR)/str_input.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/onegin.o: $(DIR_ONEGIN_SOUR)/onegin.cpp $(DIR_ONEGIN_SOUR)/str_input.h $(DIR_ONEGIN_SOUR)/func_sort.h $(DIR_LIB)/str_func.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/str_input.o: $(DIR_ONEGIN_SOUR)/str_input.cpp $(DIR_ONEGIN_SOUR)/str_input.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/func_sort.o: $(DIR_ONEGIN_SOUR)/func_sort.cpp $(DIR_ONEGIN_SOUR)/func_sort.h $(DIR_ONEGIN_SOUR)/str_input.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/str_output.o: $(DIR_ONEGIN_SOUR)/str_output.cpp $(DIR_ONEGIN_SOUR)/func_sort.h $(DIR_ONEGIN_SOUR)/str_input.h $(DIR_ONEGIN_SOUR)/str_output.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/len_array.o: $(DIR_ONEGIN_SOUR)/len_array.cpp $(DIR_ONEGIN_SOUR)/func_sort.h $(DIR_ONEGIN_SOUR)/str_input.h
	$(CC) $(CFLAGS) $< -o $@

$(DIR_O)/algoritm_sort.o: $(DIR_ONEGIN_SOUR)/algoritm_sort.cpp $(DIR_ONEGIN_SOUR)/func_sort.h $(DIR_ONEGIN_SOUR)/algoritm_sort.h
	$(CC) $(CFLAGS) $< -o $@




