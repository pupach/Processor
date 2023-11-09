#ifndef DIS_ASSEMBLER
#define DIS_ASSEMBLER
#include "command.h"
#include "../Stack/stack.h"
#include "../str_project/onegin.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_SIZE_STR_INT = 11;

struct Dis_assembler
{
    len_arr *list_labels = {};//???
    len_arr *ptr_on_code = {};
    len_arr *arr_com_ass = {};
    int command_assembling = -1;
    /*
    char *last_var_str_com = nullptr;
    int last_var_int_com = POISON_VAL;*/
};

static bool check_version(int file_version);

static len_arr *read_bin_file(FILE *stream_read);


len_arr *compile_one_str_dis(Dis_assembler *dis_ass);

CODE_ERRORS compiling_dis_assembler(FILE *stream_read, FILE *stream_write);

CODE_ERRORS dis_assembling(char file_to_read[], char file_to_write[]);
#endif
