#include "../my_lib/work_with_file.h"
#include "../str_project/source/len_array.h"
#include "../str_project/onegin.h"
#include "../my_lib/str_func.h"
#include "command.h"
#include "Processor.h"
#include "dis_assembler.h"
#include <stdlib.h>

static const size_t MAX_SIZE_COM = 10;

static const int VERSION_PROGRAMM = 20;

static bool check_version(int file_version);

#define DEF_COM(command, numb)\
static len_arr *com_##command()\
{\
    char *ptr_res_str;\
    int res_size = asprintf(&ptr_res_str, #command"\n");\
    return gen_struct_len_arr(ptr_res_str, res_size);\
}


#define DEF_COM_WITH_ARGS(command, numb, ...)\
static len_arr *com_##command(Dis_assembler *dis_ass)\
{\
    char *ptr_res_str;\
    char *ptr_inter_str = nullptr;\
    size_t res_size = 0;\
\
    int *com_for_dis_ass = ((int*)dis_ass->ptr_on_code->arr) + dis_ass->command_assembling;\
    COMMAND_ASSEMBL * cur_com = find_com_in_arr_com(dis_ass->arr_com_ass, numb);\
    DEP_COM *arr_dep_com = (DEP_COM *)cur_com->depend_com->arr;\
\
    for(int i = 0; i < cur_com->depend_com->size_arr; i++)\
    {\
        LOG(1, stderr, "for begin %d  %s  byte for check %d  %d res if %d\n", i, arr_dep_com[i].ptr_this_com->str_code, *com_for_dis_ass, arr_dep_com[i].bit_com, (arr_dep_com[i].bit_com &  *com_for_dis_ass));\
        if((arr_dep_com[i].bit_com &  *com_for_dis_ass) ==  arr_dep_com[i].bit_com)\
        {\
            res_size = asprintf(&ptr_inter_str, "%s", arr_dep_com[i].ptr_this_com->str_code);\
            LOG(1, stderr, "\n com_to_bin "#command"  %s\n", arr_dep_com[i].ptr_this_com->str_code);\
        }\
        LOG(1, stderr, "for end\n");\
    }\
    if(ptr_inter_str == nullptr)\
    {\
        res_size = asprintf(&ptr_inter_str, "stk");\
    }\
    res_size = asprintf(&ptr_res_str, "%s %s %d\n", cur_com->str_code, ptr_inter_str, *(com_for_dis_ass+1));\
    free(ptr_inter_str);\
    __VA_ARGS__;\
    return gen_struct_len_arr(ptr_res_str, res_size);\
}

#include"DSL.h"

#undef DEF_COM
#undef DEF_COM_WITH_ARGS

CODE_ERRORS dis_assembling(char file_to_read[], char file_to_write[])
{
    FILE *stream_read = open_file(file_to_read, "r");
    FILE *stream_write = open_file(file_to_write, "wb");

    len_arr *prt_all_command = read_bin_file(stream_read);

    printf_len_arr_in_LOG_int(prt_all_command, stderr);

    Dis_assembler *dis_ass = init_dis_assembler(prt_all_command, 10);

    compiling_dis_assembler(dis_ass, stream_write);

    fclose(stream_write);

    free_all_dinamic_ptr(prt_all_command);
    HADLER_EROR(free_mem_dis_ass(dis_ass));
}

Dis_assembler *init_dis_assembler(len_arr *ptr_on_code, size_t numb_label)
{
    Dis_assembler *dis_ass = (Dis_assembler *)calloc(sizeof(Dis_assembler), 1);
    dis_ass->ptr_on_code = ptr_on_code;
    dis_ass->command_assembling = 0;

    label *arr_label = (label *)calloc(sizeof(label), numb_label);
    dis_ass->list_labels = gen_struct_len_arr(arr_label, numb_label);
    dis_ass->list_labels->size_arr = 0;

    dis_ass->arr_com_ass = fill_list_assembl_command();
    dis_ass->arr_com_ass = fill_dep_com(dis_ass->arr_com_ass);

    for(int i = 0; i < dis_ass->arr_com_ass->size_arr; i++)
    {
        LOG(1, stderr, "for init %d", i);
        LOG(1, stderr, "arr_com_ass %d  %s  depend_com size_dep_com %d", i, ((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].str_code, ((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com->size_arr);
        for(int j = 0; j < ((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com->size_arr; j++)
        {
            LOG(1, stderr, "    %p   %p   ", ((DEP_COM *)((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com->arr)[j].ptr_this_com, ((DEP_COM *)((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com->arr)[j].ptr_this_com);
        }
    }
    return dis_ass;
}

static len_arr *read_bin_file(FILE *stream_read)
{
    int version_bin_arr;
    fread(&version_bin_arr, sizeof(int), 1, stream_read);

    LOG(1, stderr, "version %d\n", version_bin_arr);

    if (!check_version(version_bin_arr)) return nullptr;

    LOG(1, stderr, "not nullptr\n");

    int size_bin_arr;
    fread(&size_bin_arr, sizeof(int), 1, stream_read);

    int *bin_arr = (int *)calloc(size_bin_arr, sizeof(int));
    fread(bin_arr, sizeof(int), size_bin_arr, stream_read);

    return gen_struct_len_arr(bin_arr, size_bin_arr);
}


CODE_ERRORS free_mem_dis_ass(Dis_assembler * dis_ass)
{
    for(int i = 0; i < dis_ass->arr_com_ass->size_arr; i++)
    {
        if(((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com != nullptr)
        {
            free(((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com->arr);
        }
        free(((COMMAND_ASSEMBL *)dis_ass->arr_com_ass->arr)[i].depend_com);
    }
    free(dis_ass->arr_com_ass->arr);
    free(dis_ass->arr_com_ass);
    free(dis_ass->list_labels->arr);
    free(dis_ass->list_labels);
    free(dis_ass);
}


CODE_ERRORS compiling_dis_assembler(Dis_assembler *dis_ass, FILE *stream_write)
{
    char *arr_to_write_bin = (char *)calloc((dis_ass->ptr_on_code->size_arr * 15) + 5, sizeof(char));// спросить как делать лучше дефайн для печати тк может быть лен_арр или инт
    len_arr *bin_len_arr = gen_struct_len_arr(arr_to_write_bin, 0);//исправить числа
    len_arr *for_ret_str = nullptr;

    char VERSION_DIS_ASSEMBLER_STR[4] = {};

    sprintf(VERSION_DIS_ASSEMBLER_STR, "%d", VERSION_PROGRAMM);
    strcpy((char *)bin_len_arr->arr, VERSION_DIS_ASSEMBLER_STR);
    bin_len_arr->size_arr = strlen(VERSION_DIS_ASSEMBLER_STR) + 1;
    ((char *)bin_len_arr->arr)[bin_len_arr->size_arr - 1] = '\n';

    LOG(1, stderr, "bin_len_arr size_arr %d arr %s\n", bin_len_arr->size_arr , (char *)bin_len_arr->arr);

    while(dis_ass->command_assembling < dis_ass->ptr_on_code->size_arr)
    {
        for_ret_str = compile_one_str_dis(dis_ass);
        if ((for_ret_str != 0) and (for_ret_str != nullptr))
        {
            len_arr_merge_char(bin_len_arr, for_ret_str);
            printf_len_arr_in_LOG_char(for_ret_str, stderr);
            free(for_ret_str->arr);
            free(for_r  et_str);
        }
        dis_ass->command_assembling++;
    }

//    write_in_file_fwrite(bin_len_arr);
    printf_len_arr_in_LOG_char(bin_len_arr, stderr);
    fprintf(stream_write, "%s", (char *)bin_len_arr->arr);

    free(bin_len_arr);
    free(arr_to_write_bin);
}


bool check_version(int file_version)
{
    if (file_version == VERSION_PROGRAMM) return true;
    return false;
}

len_arr *compile_one_str_dis(Dis_assembler *dis_ass)
{
    const size_t MAX_SIZE_COM = 10;

    fprintf(stderr, "compile_one_str in dis_assembler\n");
    int *ptr_com = (int *)dis_ass->ptr_on_code->arr;
    int command = ptr_com[dis_ass->command_assembling];

    command = ((command << 16) >> 16);

    switch (command)
    {
        #define DEF_COM(com_gen, numb,...)\
            case numb:\
            {\
                return com_##com_gen();\
                break;\
            };
        #define DEF_COM_WITH_ARGS(com_gen, numb, ...)    \
            case numb:\
            {\
                return com_##com_gen(dis_ass);\
                break;\
            };

        #include"DSL.h"
        #undef DEF_COM_WITH_ARGS
        #undef DEF_COM
        default:
            LOG(1, stderr, "very strange error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11\n ");
            return nullptr;

    };
    return nullptr;
}
