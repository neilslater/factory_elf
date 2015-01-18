// ext/factory_elf/struct_office.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Office structs
//

#ifndef STRUCT_OFFICE_H
#define STRUCT_OFFICE_H

#include <ruby.h>
#include "narray.h"
#include "shared_vars.h"
#include "struct_schedule.h"
#include "struct_toy.h"

typedef struct _office_raw {
    volatile VALUE narr_toys;
    int * toys_ptr;
    volatile VALUE narr_toys_size_idx;
    int * toys_size_idx_ptr;
    int index_max;
  } Office;

Office *office__create();

void office__init_arrays( Office *office );

void office__gc_mark( Office *office );

void office__destroy( Office *office );

int office__find_toy( Office *office, Schedule *schedule, int min_id, int max_abs_mins, int max_size, int min_size );

void office__compress_size_index( Office *office, Schedule *schedule );

int office__count_toys_between( Office *office, Schedule *schedule, int min_size, int max_size );

long office__sum_toy_sizes_between( Office *office, Schedule *schedule, int min_size, int max_size );

#endif
