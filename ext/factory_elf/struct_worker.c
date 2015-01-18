// ext/ru_ne_ne/struct_worker.c

#include "struct_worker.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating Worker structs
//

static double sixtieth = 1.0/60.0;
static double prod_increases[2400];
static double prod_decreases[2400];

Worker *worker__create() {
  Worker *worker;
  worker = xmalloc( sizeof(Worker) );

  worker->id = 1;
  worker->productivity = 1.0;
  worker->ready_abs_mins = 9 * 60;

  return worker;
}

void worker__destroy( Worker *worker ) {
  xfree( worker );
  // VALUEs are not cleared here, they need to be left for GC to handle
  return;
}

// Called by Ruby's GC, we have to mark all child objects that could be in Ruby
// space, so that they don't get deleted.
void worker__gc_mark( Worker *worker ) {
  // Nothing to do
  return;
}

/*
 * Interesting numbers:
 * 1.02 ** 1 * 0.9 ** 0.18795112355 = 1.0000000000004838   ->  every 1 hour training is offset by 0.18.. hours overtime
 * 1.02 ** 70 = 3.99955822284844 -> it takes 7 days training to maximise output when starting from 1.0 productivity
 */

double worker__calc_new_productivity( Worker *worker, int mins_sanctioned, int mins_over ) {
  // Always true in practice, you cannot accrue 2400+ unsanctioned minutes alongside enough
  // sanctioned minutes to make any difference to the end result, productivity = 0.25
  if (mins_over >= 2399) {
    return 0.25;
  }
  double new_prod = worker->productivity * prod_increases[mins_sanctioned] * prod_decreases[mins_over];
  if (new_prod > 4.0) {
    new_prod = 4.0;
  } else if (new_prod < 0.25) {
    new_prod = 0.25;
  }
  return new_prod;
}

int worker__calc_build_time_mins( Worker *worker, int standard_toy_build_mins ) {
  return (int) ceil( standard_toy_build_mins / worker->productivity );
}

void worker__update_for_toy_build( Worker *worker, int schedule_abs_mins, int standard_toy_build_mins ) {
  int start_build_time = worker->ready_abs_mins;
  if ( schedule_abs_mins > 0 ) {
    if ( schedule_abs_mins >= start_build_time ) {
      start_build_time = schedule_abs_mins;
    } else {
      rb_raise( rb_eArgError, "Schedule time requested %d, but worker not available until %d", schedule_abs_mins, start_build_time );
    }
  }
  int mins_build = worker__calc_build_time_mins( worker, standard_toy_build_mins );
  int end_build_time = start_build_time + mins_build - 1;
  int mins_sanctioned = clock__sanctioned_minutes_between( start_build_time, end_build_time );
  worker->ready_abs_mins = clock__add_work_minutes( start_build_time, mins_build );
  worker->productivity = worker__calc_new_productivity( worker, mins_sanctioned, mins_build - mins_sanctioned );
  return;
}

double worker__predict_productivity( Worker *worker, int start_build_time, int standard_toy_build_mins ) {
  int mins_build = (int) ceil( standard_toy_build_mins / worker->productivity );
  int end_build_time = start_build_time + mins_build - 1;
  int mins_sanctioned = clock__sanctioned_minutes_between( start_build_time, end_build_time );
  return worker__calc_new_productivity( worker, mins_sanctioned, mins_build - mins_sanctioned );
}


void worker__skip_to( Worker *worker, int abs_mins ) {
  if ( worker->ready_abs_mins > abs_mins ) {
    rb_raise( rb_eArgError, "Schedule time requested %d, but worker not available until %d", abs_mins, worker->ready_abs_mins );
  }
  worker->ready_abs_mins = abs_mins;
  return;
}

int worker__max_sanctioned_size( Worker *worker ) {
  int mins_left = clock__sanctioned_minutes_remaining( worker->ready_abs_mins );
  return (int) floor( mins_left * worker->productivity );
}

int worker__max_no_loss_size( Worker *worker ) {
  int mins_left = clock__sanctioned_minutes_remaining( worker->ready_abs_mins );
  return (int) ( floor( mins_left * worker->productivity ) + floor( mins_left * worker->productivity * 0.18795112355 ) );
}

int worker__best_training_size( Worker *worker, double target ) {
  int mins_left = clock__sanctioned_minutes_remaining( worker->ready_abs_mins );
  double max_productivity = worker->productivity * prod_increases[mins_left];
  if ( max_productivity <= target ) {
    return worker__max_sanctioned_size( worker );
  }
  return ceil( worker->productivity * ( 60.0 * log( target / worker->productivity ) ) / 0.01980262729617973 );
}

void worker__init_cache() {
 int i;
 for( i = 0; i < 2400; i++ ) {
  prod_increases[i] = pow( 1.02, sixtieth * i );
  prod_decreases[i] = pow( 0.9, sixtieth * i );
 }
 return;
}