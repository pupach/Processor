#ifndef PROCESSOR
#define PROCESSOR
#include "command.h"
#include "../Stack/source/stack.h"
#include "../str_project/onegin.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const int AMOUNT_VAR_IN_PROC = 4;

static const int MAX_SIZE_LABEL_NAME = 10;

struct Processor
{
    Stack *stk = nullptr;
    bool flag_hlt = false;
    len_arr *ptr_on_com = {};
    len_arr *arr_com_ass = {};
    int variables[AMOUNT_VAR_IN_PROC] = {};
    int numb_com_processing = -1;
};

struct COMMAND_ASSEMBL
{
    int int_code = 0;
    const char *str_code = nullptr;
    len_arr *depend_com = nullptr;
};

struct DEP_COM
{
    int bit_com = -1;
    COMMAND_ASSEMBL *ptr_this_com = nullptr;
};

struct label
{
    char name[MAX_SIZE_LABEL_NAME]= {};
    int numb_str_code = -1;
};

Processor *init_processor(len_arr *ptr_on_com);

len_arr *fill_dep_com(len_arr *arr_com_ass);

len_arr *fill_list_assembl_command();

COMMAND_ASSEMBL *find_com_in_arr_com(len_arr *arr_com_ass, int numb_com);

static bool check_version(int file_version);

#endif
