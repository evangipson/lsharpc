#include "run_lsharp.h"

void run_lsharp(statement* statements)
{
    /* reset the state of the L# state machine before running the program */
    lsharp_state* state = create_lsharp_state();
    reset_lsharp_state(state);

    return;
}