#include "../my_lib/work_with_file.h"
#include "../str_project/onegin.h"
#include "command.h"
#include "assembler.h"
#include "dis_assembler.h"
#include "virtual_machine.h"
#include "Processor.h"

//TO DO: struct commands with all info


COMMAND_ASSEMBL *find_com_in_arr_com(len_arr *arr_com_ass, int numb_com)
{
    assert(arr_com_ass != nullptr);
    assert(arr_com_ass->arr != nullptr);

    COMMAND_ASSEMBL *ass_arr_com = (COMMAND_ASSEMBL *)arr_com_ass->arr;
    for(int i = 0; i < arr_com_ass->size_arr; i++)
    {
//        LOG(1, stderr, "find_com_in_arr_com, numb_com %d, int_code %d \n", numb_com, ass_arr_com[i].int_code);
        if(ass_arr_com[i].int_code == numb_com)
        {
            return &ass_arr_com[i];
        }
    }
    return nullptr;
}


len_arr *fill_list_assembl_command()
{
    int size_arr = 10;
    int am_com_ass = 0;
    COMMAND_ASSEMBL * com_assembl = (COMMAND_ASSEMBL *)calloc(sizeof(COMMAND_ASSEMBL), size_arr);

    LOG(1, stderr, "init_begin\n");

    #define DEF_COM(comm, numb)\
        if(size_arr <= am_com_ass) \
        {\
            size_arr = size_arr * 2;\
            com_assembl = (COMMAND_ASSEMBL *)realloc(com_assembl, sizeof(COMMAND_ASSEMBL) * size_arr);\
        }\
        com_assembl[am_com_ass].int_code = numb;\
        com_assembl[am_com_ass].str_code = #comm;\
        com_assembl[am_com_ass].depend_com = (len_arr *)calloc(sizeof(len_arr), 1);\
        am_com_ass++;

    #define DEF_COM_WITH_ARGS(comm, numb, am_dep_com, arr_dep_com, ...)\
        if(size_arr <= am_com_ass) \
        {\
            size_arr = size_arr * 2;\
            com_assembl = (COMMAND_ASSEMBL *)realloc(com_assembl, sizeof(COMMAND_ASSEMBL) * size_arr);\
        }\
        com_assembl[am_com_ass].int_code = numb;\
        com_assembl[am_com_ass].str_code = #comm;\
        am_com_ass++;

    #include "DSL.h"

    #undef DEF_COM_WITH_ARGS
    #undef DEF_COM

    LOG(1, stderr, "one include skip in init\n");

    return gen_struct_len_arr(com_assembl, am_com_ass);
}


len_arr *fill_dep_com(len_arr *arr_com_ass)
{
    #define DEF_COM(...)


    #define DEF_COM_WITH_ARGS(comm, numb, am_dep_com, arr_dep_com, ...)         \
        COMMAND_ASSEMBL *cur_com##comm = find_com_in_arr_com(arr_com_ass, numb);\
        LOG(1, stderr, "depend_com gen in init,"#comm"\n");\
        DEP_COM *depend_com##comm = (DEP_COM *)calloc(sizeof(DEP_COM), am_dep_com);\
        for(int i = 0; i < am_dep_com; i++)\
        {\
            depend_com##comm[i].bit_com = (1 << arr_dep_com[i][1]);\
            depend_com##comm[i].ptr_this_com = find_com_in_arr_com(arr_com_ass, arr_dep_com[i][0]);\
            LOG(1, stderr, "for end %d "#comm" find_com_in_arr_com %s\n numb_byte %d \n", i, find_com_in_arr_com(arr_com_ass, arr_dep_com[i][0])->str_code, arr_dep_com[i][1]);\
        }\
        cur_com##comm->depend_com = gen_struct_len_arr(depend_com##comm, am_dep_com);\
        for(int i = 0; i < am_dep_com; i++)\
        {\
            LOG(1, stderr, #comm"   %s  ", ((DEP_COM *)cur_com##comm->depend_com->arr)[i].ptr_this_com->str_code);\
        }\
        LOG(1, stderr, "cur_com  "#comm"%p \n", cur_com##comm);\

    #include "DSL.h"

    #undef DEF_COM_WITH_ARGS
    #undef DEF_COM

    return arr_com_ass;
}


// TODO: split into several programms
int main()
{
    assembling("rools.txt", "rools_bit.bin");
    dis_assembling("rools_bit.bin", "rools_check.txt");
    virtual_machining("rools_bit.bin");
}
