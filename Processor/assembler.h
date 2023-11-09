#ifndef ASSEMBLER
#define ASSEMBLER
#include "command.h"
#include "Processor.h"
#include "../Stack/stack.h"
#include "../str_project/onegin.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static bool check_version(int file_version);

static const size_t MAX_SIZE_COM = 10;

static const size_t MAX_AMOUNT_DEPEND_COM = 10;

struct Assembler
{
    len_arr *list_labels = {};
    len_arr *ptr_on_code = {};
    len_arr *arr_com_ass = {};
    int str_assembl = -1;
    int command_assembling = -1;
    char *last_var_str_com = nullptr;
    int last_var_int_com = POISON_VAL;
};

len_arr *compile_one_str_ass(Assembler *ptr_str);

CODE_ERRORS free_mem_ass(Assembler * ass);

CODE_ERRORS compiling_assembler(Assembler *ass, FILE *stream_write);

CODE_ERRORS find_all_labels(Assembler *ass);

CODE_ERRORS write_bin_file(len_arr *arr_to_write_bin, FILE *stream_write);

CODE_ERRORS assembling(char file_to_read[], char file_to_write[]);
#endif
