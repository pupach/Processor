#ifndef COMMAND
#define COMMAND
#include "../Stack/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DEF_COM(command, numb,...)  \
    command = numb,

#define DEF_COM_WITH_ARGS(command, numb,...)  \
    command = numb,

#define OUT_FUNC

enum INT_CODE_COMMAND
{
    #include"DSL.h"
};

#undef OUT_FUNC
#undef DEF_COM
#undef DEF_COM_WITH_ARGS
#endif
