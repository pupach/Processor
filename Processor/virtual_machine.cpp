#include "../my_lib/work_with_file.h"
#include "../str_project/onegin.h"
#include "../str_project/source/len_array.h"
#include "command.h"
#include "virtual_machine.h"
#include "Processor.h"


#define GENERATE_FUNC_OPERATOR(operator, suf_func)      \
static CODE_ERRORS com_##suf_func(Processor *cpu)       \
{                                                       \
    Stack *stk = cpu->stk;                              \
    Elen_s slag_1 = -1;                                 \
    Elen_s slag_2 = -1;                                 \
    Stack_Pop(stk, &slag_1);                            \
    Stack_Pop(stk, &slag_2);                            \
    LOG(1, stderr, "com_"#suf_func" slag: %d, %d", slag_1, slag_2); \
    if ((!isnan(slag_1)) and (!isnan(slag_2)))          \
    {                                                   \
       Stack_Push(stk, slag_1 operator slag_2);         \
        return ALL_GOOD;                                \
    }                                                   \
    else                                                \
    {                                                   \
       LOG(1, stderr, "UNDEF_ERROR");                   \
       return UNDEF_ERROR;                              \
    }                                                   \
}                                                       \

GENERATE_FUNC_OPERATOR(+, add);

GENERATE_FUNC_OPERATOR(*, mul);

GENERATE_FUNC_OPERATOR(-, sub);

GENERATE_FUNC_OPERATOR(/, divis);

static const int VERSION_PROGRAMM = 20;

static bool check_version(FILE *stream_read);

static CODE_ERRORS com_jump(Processor *cpu, COMMAND_ASSEMBL *jump_com);

static CODE_ERRORS com_pop(Processor *cpu, COMMAND_ASSEMBL *pop_com);

static CODE_ERRORS com_push(Processor *cpu, COMMAND_ASSEMBL *push_com);

static CODE_ERRORS com_out(Processor *cpu);

static CODE_ERRORS com_HLT(Processor *cpu);

static CODE_ERRORS com_labels(Processor *cpu, COMMAND_ASSEMBL *label_com);




CODE_ERRORS virtual_machining(char file_to_read[])
{
    FILE *stream_read = open_file(file_to_read, "r");

    run_program_from_file(stream_read);
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


CODE_ERRORS run_program_from_file(FILE *stream_read)
{
    len_arr *prt_all_command = read_bin_file(stream_read);

    printf_len_arr_in_LOG_int(prt_all_command, stderr);

    Processor *Cpu = init_processor(prt_all_command);

    execute_all_command(Cpu);

    Stack_Destructor(Cpu->stk);
    free_all_dinamic_ptr(prt_all_command);
    free(Cpu);
}

Processor *init_processor(len_arr *ptr_on_com)
{
    Processor *Cpu = (Processor *)calloc(sizeof(Processor), 1);

    Stack stk1 = {};
    Stack *stk2 = &stk1;
    Stack_init(stk2, 20, 2);

    Cpu->stk = stk2;
    Cpu->numb_com_processing = 0;
    Cpu->ptr_on_com = ptr_on_com;
    Cpu->arr_com_ass = fill_list_assembl_command();
    Cpu->arr_com_ass = fill_dep_com(Cpu->arr_com_ass);
    return Cpu;

}

CODE_ERRORS execute_all_command(Processor *Cpu)
{
    bool flag_hlt = false;
    #define OUT_FUNC

    #define DEF_COM_WITH_ARGS(com, numb, ...)\
        COMMAND_ASSEMBL *com##_com = find_com_in_arr_com(Cpu->arr_com_ass, numb);

    #define DEF_COM(com, numb)

    #include "DSL.h"

    #undef OUT_FUNC
    #undef DEF_COM_WITH_ARGS
    #undef DEF_COM

    do
    {
        int *ptr_com = (((int *)Cpu->ptr_on_com->arr));
        int command = ptr_com[Cpu->numb_com_processing];
        command = ((command << 16) >> 16);

        LOG(1, stderr, "\nexecute_all_str %d count %d\n", command, Cpu->numb_com_processing);

        switch (command)
        {
        #define MACHINING
        #define DEF_COM(com_gen, numb,...)\
            case numb:\
            {\
                com_##com_gen(Cpu);\
                break;\
            };

        #define DEF_COM_WITH_ARGS(com_gen, numb, ...)    \
            case numb:\
            {\
                LOG(1, stderr, "comand to machining, %d\n", command);\
                com_##com_gen(Cpu, com_gen##_com);\
                break;\
            };
        #include"DSL.h"
        #undef DEF_COM_WITH_ARGS
        #undef DEF_COM
        #undef MACHINING
            default:
                LOG(1, stderr, "very strange error undef command, %d\n", command);
                Stack_Destructor(Cpu->stk);
                return UNDEF_ERROR;
        };
        Cpu->numb_com_processing++;
    }while(((Cpu->ptr_on_com->size_arr) > Cpu->numb_com_processing) and !(Cpu->flag_hlt));

    return ALL_GOOD;
}

bool check_version(int file_version)
{
    if (file_version == VERSION_PROGRAMM) return true;
    return false;
}


static CODE_ERRORS com_push(Processor *cpu, COMMAND_ASSEMBL *push_com)
{
    int *ptr_com = (((int *)cpu->ptr_on_com->arr));
    int command = ptr_com[cpu->numb_com_processing + 1];
    int numb_va = ptr_com[cpu->numb_com_processing + 2];

    if(command & (((DEP_COM *)push_com->depend_com->arr)[0].bit_com))
    {
        if ((numb_va > 0) and (numb_va < AMOUNT_VAR_IN_PROC))
        {
            Elen_s slag_1 = -1;
            Stack_Pop(cpu->stk, &slag_1);
            cpu->variables[numb_va - 1] = slag_1;

            LOG(1, stderr, "com_push number to push in var %d, %d val in var %d", numb_va - 1, slag_1, cpu->variables[numb_va - 1]);

            cpu->numb_com_processing += 2;
            return TWO_VAL;
        }
    }
    else if(command & (((DEP_COM *)push_com->depend_com->arr)[1].bit_com))
    {
        cpu->numb_com_processing += 1;
        Stack_Push(cpu->stk, (Elen_s)numb_va);

        return ALL_GOOD;
    }
    return UNDEF_COM;
}

static CODE_ERRORS com_pop(Processor *cpu, COMMAND_ASSEMBL *pop_com)
{
    int *ptr_com = (((int *)cpu->ptr_on_com->arr));
    int command = ptr_com[cpu->numb_com_processing + 1];
    int numb_va = ptr_com[cpu->numb_com_processing + 2];

    if(command & (((DEP_COM *)pop_com->depend_com->arr)[0].bit_com))
    {
        if ((numb_va > 0) and (numb_va < AMOUNT_VAR_IN_PROC))
        {
            Stack_Push(cpu->stk, cpu->variables[numb_va]);

            cpu ->numb_com_processing += 2;
            return TWO_VAL;
        }
    }
    return UNDEF_COM;
    /*
    else if(command & (((DEP_COM *)pop_com->depend_com->arr)[1].bit_com))
    {
        Cpu_proc->numb_com_processing += 1;
        Stack_Push(cpu->stk, (Elen_s)numb_va);

        return ALL_GOOD;
    }
    */
}

static CODE_ERRORS com_jump(Processor *cpu, COMMAND_ASSEMBL *jump_com)
{

    int *ptr_com = (((int *)cpu->ptr_on_com->arr));
    int command = ptr_com[cpu->numb_com_processing + 1];
    int numb_va = ptr_com[cpu->numb_com_processing + 2];

    if(command & (((DEP_COM *)jump_com->depend_com->arr)[0].bit_com))
    {
        cpu->numb_com_processing = (numb_va - 1);
        cpu->numb_com_processing += 1;

        return ALL_GOOD;
    }
    return UNDEF_COM;
}

static CODE_ERRORS com_out(Processor *cpu)
{
    Stack *stk = cpu->stk;
    Elen_s slag_1 = -1;
    Stack_Pop(stk, &slag_1);

    if (!isnan(slag_1))
    {
       printf("element number %d equ %ld", stk->size_s, slag_1);
    }
    else
    {
       LOG(1, stderr, "UNDEF_ERROR");
       return UNDEF_COM;
    }
}

static CODE_ERRORS com_HLT(Processor *cpu)
{
    cpu->flag_hlt = true;
    return ALL_GOOD;
}

static CODE_ERRORS com_labels(Processor *cpu, COMMAND_ASSEMBL *label_com)
{
    return UNDEF_COM;
}


