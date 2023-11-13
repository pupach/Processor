#define ASSEMBLING
#include "../my_lib/work_with_file.h"
#include "../my_lib/str_func.h"
#include "../str_project/onegin.h"
#include "../my_lib/len_arr/len_array.h"
#include "command.h"
#include "Processor.h"
#include "assembler.h"

static const int VERSION_PROGRAMM = 21;

static bool check_version(FILE *stream_read);

static int find_label_by_name(Assembler *ass, char *name);

static const char *NUMB_COM[512] = {};

#define OUT_FUNC

#define DEF_COM(command, numb, ...)  \
static len_arr *com_##command()\
{\
    int arr_ret[1] = {command};\
    return gen_struct_len_arr(arr_ret, 1);\
}



#define DEF_COM_WITH_ARGS(command, numb, ...) /*реализовать вынимание команд из ассемблера и их анализ в частоности для пуша и попа  для джампа через ва_арг*/\
static len_arr *com_##command(Assembler *ass)\
{\
    int var1 = atoi(ass->cur_com_args[0]);\
    int com_to_bin = command;\
    int first_numb_com = 0;\
\
    LOG(1, stderr, "s %d  command_what"#command" n", var1);\
\
    COMMAND_ASSEMBL *cur_com = find_com_in_arr_com(ass->arr_com_ass, numb);\
    DEP_COM *arr_dep_com = (DEP_COM *)cur_com->depend_com->arr;\
    LOG(1, stderr, "arr_dep_com.size_arr= %d\n", cur_com->depend_com->size_arr);\
    for(int i = 0; i < cur_com->depend_com->size_arr; i++)\
    {\
        LOG(1, stderr, "for begin %d  arr_dep_com %p   %s\n", i, arr_dep_com[i].ptr_this_com, arr_dep_com[i].ptr_this_com->str_code);\
        for(size_t j = 0; (j < MAX_ANOUNT_ARGUMENTS and ass->cur_com_args[j] != nullptr); j++)\
        {\
            if(strcmp(arr_dep_com[i].ptr_this_com->str_code, ass->cur_com_args[j]) == 0)\
            {\
                com_to_bin += arr_dep_com[i].bit_com;\
                first_numb_com = i;\
\
                LOG(1, stderr, "\n com_to_bin "#command"var %s\n", ass->cur_com_args[j]);\
            }\
            LOG(1, stderr, "for end\n");\
        }\
    }\
    int arr_ret[MAX_ANOUNT_ARGUMENTS + 1] = {};\
    arr_ret[0] = com_to_bin;\
    size_t counter = 1;\
\
    __VA_ARGS__;\
    LOG(1, stderr, "fill arr_ret    first_numb_com = %d\n", first_numb_com);\
\
    while((first_numb_com < MAX_ANOUNT_ARGUMENTS) and (ass->cur_com_args[first_numb_com] != nullptr))\
    {\
        LOG(1, stderr, "counter=%d, str = %s numb=%d\n", counter, ass->cur_com_args[first_numb_com], atoi(ass->cur_com_args[first_numb_com]));\
\
        arr_ret[counter] = atoi(ass->cur_com_args[first_numb_com]);\
        counter++;\
        first_numb_com++;\
    }\
\
    return gen_struct_len_arr(arr_ret, counter);\
}

#include"DSL.h"
#undef OUT_FUNC
#undef DEF_COM
#undef DEF_COM_WITH_ARGS

Assembler *init_assembler(len_arr *ptr_on_code, size_t numb_label)
{
    Assembler *ass = (Assembler *)calloc(sizeof(Assembler), 1);//у каждого ассумбрела свой отдельный код
    ass->ptr_on_code = cpy_onegin_len_arr(ptr_on_code);          //как сделать копирование более адекватным
    ass->str_assembl = 0;
    ass->command_assembling = 0;

    printf_onegin_txt(ass->ptr_on_code);

    label *arr_label = (label *)calloc(sizeof(label), numb_label);
    ass->list_labels = gen_struct_len_arr(arr_label, numb_label);
    ass->list_labels->size_arr = 0;

    ass->arr_com_ass = fill_list_assembl_command();
    ass->arr_com_ass = fill_dep_com(ass->arr_com_ass);

    ass->cur_com_args = (char **)calloc(sizeof(char *), MAX_ANOUNT_ARGUMENTS);
    if(!ass->cur_com_args)      return nullptr;

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
    LOG(1, stderr, "compiling_assembler code in assembler :\n");
    printf_onegin_txt(ass->ptr_on_code);

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
    free_cpy_onegin_struct(ass->ptr_on_code);
    free(ass->arr_com_ass->arr);
    free(ass->arr_com_ass);
    free(ass->list_labels->arr);
    free(ass->list_labels);
    free(ass->cur_com_args);
    free(ass);
}

len_arr *compile_one_str_ass(Assembler *ass)
{
    len_arr *ptr_str = (((len_arr *)ass->ptr_on_code->arr) + ass->str_assembl);
    char command[MAX_SIZE_COM] = {};
    fprintf(stderr, "compile_one_str %s     ass->str_assembl = %d\n", ((char *)ptr_str->arr), ass->str_assembl);
    size_t size_1 = strcpy_to_letter(command, ((char *)ptr_str->arr));

    LOG(1, stderr, "str in %s\n", command);

    char *ptr_str_arr = (char *)ptr_str->arr + size_1;

    #define DEF_COM_WITH_ARGS(com_gen, numb, len1, list, amount_arg,...)    \
    if(strcmp(#com_gen, command) == 0)                          \
        {\
            HANDLER_ERROR(read_n_arg(amount_arg)); /*разобраться почему оно не считывает номер переменной то есть не работает atoi*/\
            \
            return com_##com_gen(ass);  \
        }\
    else

    #define DEF_COM(com_gen, numb,...)\
        if(strcmp(#com_gen, command) == 0)   \
            {\
                return com_##com_gen();  \
            }\q-
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

CODE_ERRORS read_n_arg(char *ptr_str, int amount_arg)
{
    size_t counter = 0;
    ptr_str_arr = strtok(ptr_str_arr, " ");
    while((ptr_str_arr != nullptr) and (counter < amount_arg))
    {
        ass->cur_com_args[counter] = ptr_str_arr;
        counter++;
        ptr_str_arr = strtok(NULL, " ");
    }

    for(size_t i = 0; i < counter; i++)
    {
        LOG(1, stderr, "%s  ", ass->cur_com_args[i]);
    }

    while(counter < amount_arg)
    {
        ass->cur_com_args[counter] = nullptr;
        counter++;
    }+

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
