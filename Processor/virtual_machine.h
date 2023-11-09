#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE
#include "command.h"
#include "../Stack/stack.h"
#include "../str_project/onegin.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Processor.h"



CODE_ERRORS run_program_from_file(FILE *stream_read);

static len_arr *read_bin_file(FILE *stream_read);

static bool check_version(int file_version);

CODE_ERRORS virtual_machining(char file_to_read[]);

CODE_ERRORS execute_all_command(Processor *Cpu);
#endif



