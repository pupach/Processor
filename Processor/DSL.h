#ifndef OUT_FUNC
#ifndef DOUBLE_DEF//вначале идут строковые команды и только потом номера
static const int arr_1[2][2] =  {{var, 29}, {stk, 28}}; //сделать один defaine def_com_with_arghs

static const int arr_3[4][2] =  {{jmp_b, 29}, {jmp_m, 28}, {jmp_br, 27}, {jmp_mr, 26}};

static const int arr_2[1][2] =  {{var, 29}};
#endif

DEF_COM_WITH_ARGS(push, 31, 2, arr_1, 2)

DEF_COM_WITH_ARGS(pop, 89, 1, arr_2, 2) //ИСПРАВИТЬ ЧТО ДЛИННУ МАССИВА НУЖН ПЕРЕДАВАТЬ ОТДЕЛЬНО


DEF_COM_WITH_ARGS(jump, 76, 0, arr_3, 3)

DEF_COM_WITH_ARGS(labels, 29, 0, arr_3, 1)
#else
DEF_COM_WITH_ARGS(push, 31)

DEF_COM_WITH_ARGS(pop, 89)

DEF_COM_WITH_ARGS(labels, 29, int code_str = ass->command_assembling;
                            int numb_labels = ass->list_labels->size_arr;
                            strcpy(((label *)(ass->list_labels->arr))[numb_labels].name, ass->cur_com_args[0]);
                            ((label *)ass->list_labels->arr)[numb_labels].numb_str_code = code_str;
                            LOG(1, stderr, "\ncode VA_ARG labels, %d, %d  name label %s\n", code_str, numb_labels, ((label *)ass->list_labels->arr)[numb_labels].name);
                            ass->list_labels->size_arr += 1;
                            return nullptr;)

DEF_COM_WITH_ARGS(jump, 76, LOG(1, stderr, "jump with var1 %d\n", var1);
                                if (var1 == 0)
                                {
                                    int ind = find_label_by_name(ass, ass->cur_com_args[0]);
                                    int com = ((label *)(ass->list_labels->arr))[ind].numb_str_code;
                                    int arr_ret[2] = {com_to_bin, com};
                                    LOG(1, stderr, "jump ind=%d and var2=%s str code=%d\n",ind ,ass->cur_com_args[0], com);
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

DEF_COM(jmp_b, 90)

DEF_COM(jmp_br, 91)

DEF_COM(jmp_mr, 92)

DEF_COM(jmp_m, 93)

DEF_COM(stk, 78)

DEF_COM(POISON_VAL, -100001)

DEF_COM(comment, 886)

#endif

