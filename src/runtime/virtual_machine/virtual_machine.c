#include "virtual_machine.h"

/// @brief helper function to mark an object
static void mark_object(virtual_machine* vm, int index) {
    if (vm->object_flags[index] == 0)
    {
        vm->object_flags[index] = 1;
        /**
         * TODO: If your objects contain pointers to other objects, recursively mark them here.
         * 
         * For example, if you have a linked list:
         * 
         * if (object_is_list(vm->objects[index]))
         * {
         *     mark_object(vm, get_list_head_index(vm->objects[index]));
         * }
         */
    }
}

void garbage_collect(virtual_machine* virtual_machine) {
    /* mark objects reachable from the stack */
    for (int i = 0; i < virtual_machine->stack_pointer; i++)
    {
        if (virtual_machine->stack[i].type == VAL_STRING)
        {
            /* find the index of the string in the objects array */
            for (int j = 0; j < virtual_machine->object_count; j++)
            {
                if (virtual_machine->objects[j] == virtual_machine->stack[i].as.s)
                {
                    mark_object(virtual_machine, j);
                    break;
                }
            }
        }
    }

    /* mark objects reachable from variables */
    for (int i = 0; i < virtual_machine->variable_count; i++)
    {
        if (virtual_machine->variables[i].type == VAL_STRING)
        {
            /* find the index of the string in the objects array */
            for (int j = 0; j < virtual_machine->object_count; j++)
            {
                if (virtual_machine->objects[j] == virtual_machine->variables[i].as.s)
                {
                    mark_object(virtual_machine, j);
                    break;
                }
            }
        }
    }

    /* sweep objects when they are marked */
    int new_object_count = 0;
    for (int i = 0; i < virtual_machine->object_count; i++)
    {
        if (virtual_machine->object_flags[i] == 1)
        {
            /* marked, keep it */
            virtual_machine->objects[new_object_count] = virtual_machine->objects[i];
            /* unmark it for the next GC */
            virtual_machine->object_flags[new_object_count] = 0;
            new_object_count++;
            continue;
        }

        /* not marked, free it */
        free(virtual_machine->objects[i]);
    }

    /* update object count post-sweep */
    virtual_machine->object_count = new_object_count;
}