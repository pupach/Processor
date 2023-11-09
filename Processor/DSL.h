#ifndef OUT_FUNC
#ifndef DOUBLE_DEF
static const int arr_1[2][2] =  {{var, 29}, {stk, 28}};

static const int arr_3[2][2] =  {};

static const int arr_2[2][2] =  {{var, 29}};
#endif

DEF_COM_WITH_ARGS(push, 31, 2, arr_1)

DEF_COM_WITH_ARGS(pop, 89, 2, arr_2)

DEF_COM_WITH_ARGS(jump, 76, 0, arr_3)

DEF_COM_WITH_ARGS(labels, 29, 0, arr_3)
#else
DEF_COM_WITH_ARGS(push, 31)

DEF_COM_WITH_ARGS(pop, 89)

DEF_COM_WITH_ARGS(labels, 29, int code_str = ass->command_assembling;
                            int numb_labels = ass->list_labels->size_arr;
                            strcpy(((label *)(ass->list_labels->arr))[numb_labels].name, ass->last_var_str_com);
                            ((label *)ass->list_labels->arr)[numb_labels].numb_str_code = code_str;
                            LOG(1, stderr, "code VA_ARG labels, %d, %d  name label %s", code_str, numb_labels, ((label *)ass->list_labels->arr)[numb_labels].name);
                            ass->list_labels->size_arr += 1;
                            return nullptr;)

DEF_COM_WITH_ARGS(jump, 76, LOG(1, stderr, "jump with var1 %d\n", var1);
                                if (var1 == 0)
                                {
                                    int ind = find_label_by_name(ass, var2);
                                    int com = ((label *)(ass->list_labels->arr))[ind].numb_str_code;
                                    int arr_ret[2] = {com_to_bin, com};
                                    LOG(1, stderr, "jump ind=%d and var2=%s str code=%d\n",ind ,var2, com);
                                    return gen_struct_len_arr(arr_ret, 2);
                                }
                                else
                                {
                                    int arr_ret[2] = {com_to_bin, var1};
                                    return gen_struct_len_arr(arr_ret, 2);
                                })
#endif


DEF_COM(HLT, -2)

DEF_COM(add, 5)

DEF_COM(sub, 6)

DEF_COM(mul, 7)

DEF_COM(divis, 8)

DEF_COM(out, 11)

#ifndef MACHINING //сделано, чтобы в virtual_machine эти команды при генерации дефайнов не вызывались.

DEF_COM(var, 9)

DEF_COM(stk, 78)

DEF_COM(POISON_VAL, -100001)

DEF_COM(comment, 0)

#endif
