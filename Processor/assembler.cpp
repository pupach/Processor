#define ASSEMBLING
#include "../my_lib/work_with_file.h"
#include "../my_lib/str_func.h"
#include "../str_project/onegin.h"
#include "../str_project/source/len_array.h"
#include "command.h"
#include "Processor.h"
#include "assembler.h"

static const int VERSION_PROGRAMM = 20;

static bool check_version(FILE *stream_read);

// TODO: move to assembler.h
Assembler *init_assembler(len_arr *ptr_on_code, size_t numb_label=10);

static int find_label_by_name(Assembler *ass, char *name);

static const char *NUMB_COM[512] = {};

#define OUT_FUNC

#define DEF_COM(command, numb, ...)  \
static len_arr *com_##command()\
{\
    int arr_ret[1] = {command};\
    return gen_struct_len_arr(arr_ret, 1);\
}

#define DEF_COM_WITH_ARGS(command, numb, ...) \
static len_arr *com_##command(Assembler *ass)\
{\
    int com_to_bin = command;\
    int var1 = ass->last_var_int_com;\
    char *var2 = ass->last_var_str_com;\
    LOG(1, stderr, "s %d  command_what"#command" %s \n", var1, var2);\
    COMMAND_ASSEMBL * cur_com = find_com_in_arr_com(ass->arr_com_ass, numb);\
    DEP_COM *arr_dep_com = (DEP_COM *)cur_com->depend_com->arr;\
    LOG(1, stderr, "arr_dep_com %p\n", arr_dep_com);\
    for(int i = 0; i < cur_com->depend_com->size_arr; i++)\
    {\
        LOG(1, stderr, "for begin %d  arr_dep_com %p   %s\n", i, arr_dep_com[i].ptr_this_com, arr_dep_com[i].ptr_this_com->str_code);\
        if(strcmp(arr_dep_com[i].ptr_this_com->str_code, var2) == 0)\
        {\
            com_to_bin += arr_dep_com[i].bit_com;\
            LOG(1, stderr, "\n com_to_bin "#command"var %s\n", var2);\
        }\
        LOG(1, stderr, "for end\n");\
    }\
    int arr_ret[2] = {com_to_bin, (int )var1};\
    __VA_ARGS__;\
    return gen_struct_len_arr(arr_ret, 2);\
}

#include"DSL.h"
#undef OUT_FUNC
#undef DEF_COM
#undef DEF_COM_WITH_ARGS

Assembler *init_assembler(len_arr *ptr_on_code, size_t numb_label)
{
    Assembler *ass = (Assembler *)calloc(sizeof(Assembler), 1);
    ass->ptr_on_code = ptr_on_code;
    ass->str_assembl = 0;
    ass->command_assembling = 0;

    label *arr_label = (label *)calloc(sizeof(label), numb_label);
    ass->list_labels = gen_struct_len_arr(arr_label, numb_label);
    ass->list_labels->size_arr = 0;

    ass->arr_com_ass = fill_list_assembl_command();
    ass->arr_com_ass = fill_dep_com(ass->arr_com_ass);

    for(int i = 0; i < ass->arr_com_ass->size_arr; i++)
    {
        LOG(1, stderr, "for init %d", i);
        LOG(1, stderr, "arr_com_ass %d  %s  depend_com size_dep_com %d", i, ((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].str_code, ((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com->size_arr);
        for(int j = 0; j < ((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com->size_arr; j++)
        {
            LOG(1, stderr, "    %p   %p   ", ((DEP_COM *)((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com->arr)[j].ptr_this_com, ((DEP_COM *)((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com->arr)[j].ptr_this_com);
        }
    }
    return ass;
}

//переписатьlen_arr чтобы вместо gen_struct_len_arr вызывать len_arr_merge чтобы не было calloc
CODE_ERRORS assembling(char file_to_read[], char file_to_write[])
{
    FILE *stream_read = open_file(file_to_read, "r");
    FILE *stream_write = open_file(file_to_write, "wb");

    int file_version = -1;
    fscanf(stream_read, "%d\n", &file_version);
    if (!check_version(file_version)) return BAD_VERSION;

    len_arr *buff = nullptr;
    len_arr *ptr_all_str = split_file_on_str(stream_read, &buff);
    Assembler *ass = init_assembler(ptr_all_str);
    LOG(1, stderr, "assembling list label size  %d\n", ass->list_labels->size_arr);

    HADLER_EROR(find_all_labels(ass));

    HADLER_EROR(compiling_assembler(ass, stream_write));

    fclose(stream_write);
    free_all_dinamic_ptr(ptr_all_str);
    free_mem_buf(buff);
    HADLER_EROR(free_mem_ass(ass));
}

bool check_version(int file_version)
{
    if (file_version == VERSION_PROGRAMM) return true;
    return false;
}

CODE_ERRORS find_all_labels(Assembler *ass)
{
    Assembler *ass_only_fol_label = init_assembler(ass->ptr_on_code);
    if(ass_only_fol_label == nullptr)      return PTR_ASSEMBLER_NULL;

    HADLER_EROR(compiling_assembler(ass_only_fol_label, nullptr));
    if(ass_only_fol_label->list_labels == nullptr)     {return PTR_LIST_LABELS_NULL;}
    LOG(1, stderr, "\n ass_only_fol_label->list_labels %p\n", ass_only_fol_label->list_labels);
    len_arr a = *(ass_only_fol_label->list_labels);
    if(ass_only_fol_label->list_labels->arr == nullptr){return PTR_ARR_NULL;}

    LOG(1, stderr, "find_all_labels, size_list_labels %d\n", ass_only_fol_label->list_labels->size_arr);

    memcpy(ass->list_labels->arr, (label *)(ass_only_fol_label->list_labels->arr), sizeof(label) * 1);//10 = numb_labels
    ass->list_labels->size_arr = ass_only_fol_label->list_labels->size_arr;

    HADLER_EROR(free_mem_ass(ass_only_fol_label));
}


CODE_ERRORS compiling_assembler(Assembler *ass, FILE *stream_write)
{
    int *arr_to_write_bin = (int *)calloc((ass->ptr_on_code->size_arr * 3) + 2, sizeof(int));// спросить как делать лучше дефайн для печати тк может быть лен_арр или инт
    len_arr *bin_len_arr = gen_struct_len_arr(arr_to_write_bin, 0);
    ((int *)bin_len_arr->arr)[0] = VERSION_PROGRAMM ;
    bin_len_arr->size_arr = 2;

    len_arr *for_ret_str = nullptr;
    do
    {
        for_ret_str = compile_one_str_ass(ass);
        if (for_ret_str != nullptr)
        {
            ass->command_assembling += for_ret_str->size_arr;
            len_arr_merge_int(bin_len_arr, for_ret_str);
            free(for_ret_str);
        }
        ass->str_assembl++;
    }while(ass->str_assembl < ass->ptr_on_code->size_arr);

    if(stream_write != nullptr)
    {
        HADLER_EROR(write_bin_file(bin_len_arr, stream_write));
    }

    printf_len_arr_in_LOG_int(bin_len_arr, stderr);

    free(bin_len_arr->arr);
    free(bin_len_arr);
}

CODE_ERRORS free_mem_ass(Assembler * ass)
{
    for(int i = 0; i < ass->arr_com_ass->size_arr; i++)
    {
        if(((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com != nullptr)
        {
            free(((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com->arr);
        }
        free(((COMMAND_ASSEMBL *)ass->arr_com_ass->arr)[i].depend_com);
    }
    free(ass->arr_com_ass->arr);
    free(ass->arr_com_ass);
    free(ass->list_labels->arr);
    free(ass->list_labels);
    free(ass);
}

len_arr *compile_one_str_ass(Assembler *ass)
{
    len_arr *ptr_str = (((len_arr *)ass->ptr_on_code->arr) + ass->str_assembl);
    char command[MAX_SIZE_COM] = {};
    fprintf(stderr, "compile_one_str %s\n", ((char *)ptr_str->arr));
    size_t size_1 = strcpy_to_letter(command, ((char *)ptr_str->arr));

    LOG(1, stderr, "str in %s\n", command);

    char *ptr_str_arr = ((char *)ptr_str->arr + size_1);

    #define DEF_COM_WITH_ARGS(com_gen, numb, ...)    \
    if(strcmp(#com_gen, command) == 0)                          \
        {\
            char command_what_push[MAX_SIZE_COM] = {};\
            size_t size = strcpy_to_letter(command_what_push, (char *)ptr_str_arr);\
            ptr_str_arr += size;\
            int var_to = atoi((char *)(ptr_str_arr));\
            ass->last_var_int_com = var_to;\
            ass->last_var_str_com = command_what_push;\
            return com_##com_gen(ass);  \
        }\
    else

    #define DEF_COM(com_gen, numb,...)\
        if(strcmp(#com_gen, command) == 0)   \
            {\
                return com_##com_gen();  \
            }\
        else

    #include"DSL.h"
    // else
        {
            return nullptr;
            LOG(1, stderr, "very strange error or maybe label");
        }
    #undef DEF_COM_WITH_ARGS
    #undef DEF_COM

}

CODE_ERRORS write_bin_file(len_arr *arr_to_write_bin, FILE *stream_write)
{
    ((int *)(arr_to_write_bin->arr))[1] = arr_to_write_bin->size_arr;
    fwrite(arr_to_write_bin->arr, sizeof(int), arr_to_write_bin->size_arr, stream_write);
}

static int find_label_by_name(Assembler *ass, char *name)
{
    for(int i = 0; i < ass->list_labels->size_arr; i++)
    {
        LOG(1, stderr, "\nfind_label_by_name %s, label name=%s\n", name, ((label *)(ass->list_labels->arr))[i].name);
        if(strcmp(name, ((label *)(ass->list_labels->arr))[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

#undef ASSEMBLING
