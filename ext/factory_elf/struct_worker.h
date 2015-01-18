// ext/factory_elf/struct_worker.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Worker structs
//

#ifndef STRUCT_WORKER_H
#define STRUCT_WORKER_H

#include <ruby.h>
#include "struct_clock.h"

typedef struct _worker_raw {
    int id;
    double productivity;
    int ready_abs_mins;
  } Worker;

Worker *worker__create();

void worker__destroy( Worker *worker );

// Called by Ruby's GC, we have to mark all child objects that could be in Ruby
// space, so that they don't get deleted.
void worker__gc_mark( Worker *worker );

double worker__calc_new_productivity( Worker *worker, int mins_sanctioned, int mins_over );

int worker__calc_build_time_mins( Worker *worker, int standard_toy_build_mins );

void worker__update_for_toy_build( Worker *worker, int schedule_abs_mins, int standard_toy_build_mins );

void worker__skip_to( Worker *worker, int abs_mins );

int worker__max_sanctioned_size( Worker *worker );

int worker__max_no_loss_size( Worker *worker );

int worker__best_training_size(  Worker *worker, double target );

void worker__init_cache();

double worker__predict_productivity( Worker *worker, int start_build_time, int standard_toy_build_mins );

#endif
