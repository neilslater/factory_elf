// ext/ru_ne_ne/ruby_class_worker.c

#include "ruby_class_worker.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE worker_as_ruby_class( Worker *worker, VALUE klass ) {
  return Data_Wrap_Struct( klass, worker__gc_mark, worker__destroy, worker );
}

VALUE worker_alloc(VALUE klass) {
  return worker_as_ruby_class( worker__create(), klass );
}

inline Worker *get_worker_struct( VALUE obj ) {
  Worker *worker;
  Data_Get_Struct( obj, Worker, worker );
  return worker;
}

VALUE worker_new_ruby_object( int id, double productivity, int abs_minutes ) {
  Worker *worker;
  VALUE rv_worker = worker_alloc( FactoryElf_Worker );
  worker = get_worker_struct( rv_worker );

  worker->id = id;
  worker->productivity = productivity;
  worker->ready_abs_mins = abs_minutes;

  return rv_worker;
}

void assert_value_wraps_worker( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)worker__destroy) {
    rb_raise( rb_eTypeError, "Expected a Layer object, but got something else" );
  }
}

/* Document-class:  FactoryElf::Worker
 *
 * An object of this class represents an Elf that can produce toys.
 */

//////////////////////////////////////////////////////////////////////////////////////
//
//  Worker method definitions
//

/* @overload initialize( id, productivity )
 * Creates a new FactoryElf::Worker
 * @param [Integer] id Identifier for elf when scheduling
 * @param [Integer] productivity Skill value from 0.5 to 2.0
 * @return [FactoryElf::Worker] new worker.
 */
VALUE worker_class_initialize( VALUE self, VALUE rv_id, VALUE rv_productivity ) {
  Worker *worker = get_worker_struct( self );

  worker->id = NUM2INT( rv_id );
  worker->productivity = NUM2DBL( rv_productivity );

  return self;
}

/* @overload clone
 * Cloning elves is illegal in many countries, please check with your jurisdiction.
 */
VALUE worker_class_initialize_copy( VALUE copy, VALUE orig ) {
  Worker *worker_copy;
  Worker *worker_orig;

  if (copy == orig) return copy;
  worker_copy = get_worker_struct( copy );
  worker_orig = get_worker_struct( orig );

  worker_copy->id = worker_orig->id;
  worker_copy->productivity = worker_orig->productivity;
  worker_copy->ready_abs_mins = worker_orig->ready_abs_mins;

  return copy;
}


/* @!attribute [r] id
 * Identifier for the elf.
 * @return [Integer]
 */
VALUE worker_object_id( VALUE self ) {
  Worker *worker = get_worker_struct( self );
  return INT2FIX( worker->id );
}

/* @!attribute [r] productivity
 * The current productivity rating of the worker.
 * @return [Float] between 0.25 and 4.0 inclusive
 */
VALUE worker_object_productivity( VALUE self ) {
  Worker *worker = get_worker_struct( self );
  return DBL2NUM( worker->productivity );
}

/* @!attribute [r] ready_abs_mins
 * The next time this worker is available, in minutes since 2014-01-01T00:00:00
 * @return [Integer]
 */
VALUE worker_object_ready_abs_mins( VALUE self ) {
  Worker *worker = get_worker_struct( self );
  return INT2NUM( worker->ready_abs_mins );
}

VALUE worker_object_calc_new_productivity( VALUE self, VALUE rv_mins_sanctioned, VALUE rv_mins_over ) {
  return DBL2NUM( worker__calc_new_productivity(
    get_worker_struct( self ),
    NUM2INT( rv_mins_sanctioned ),
    NUM2INT( rv_mins_over )
  ) );
}

VALUE worker_object_build_time_mins( VALUE self, VALUE rv_standard_toy_build_mins ) {
  return INT2FIX( worker__calc_build_time_mins(
    get_worker_struct( self ),
    NUM2INT( rv_standard_toy_build_mins )
  ) );
}

VALUE worker_object_update_for_toy_build( VALUE self, VALUE rv_start, VALUE rv_mins ) {
  worker__update_for_toy_build(
    get_worker_struct( self ),
    NUM2INT( rv_start ),
    NUM2INT( rv_mins ) );
  return self;
}

VALUE worker_object_skip_rest_of_day( VALUE self ) {
  Worker *worker = get_worker_struct( self );
  worker__skip_to(
    worker,
    clock__next_sanctioned_start( worker->ready_abs_mins )
  );
  return self;
}

VALUE worker_object_max_sanctioned_size( VALUE self ) {
  return INT2FIX( worker__max_sanctioned_size( get_worker_struct( self ) ) );
}

VALUE worker_object_max_no_loss_size( VALUE self ) {
  return INT2FIX( worker__max_no_loss_size( get_worker_struct( self ) ) );
}

VALUE worker_object_best_training_size( VALUE self, VALUE rv_target ) {
  return INT2FIX( worker__best_training_size( get_worker_struct( self ), NUM2DBL(rv_target ) ) );
}

VALUE worker_object_predict_productivity( VALUE self, VALUE rv_start, VALUE rv_size ) {
  Worker *worker = get_worker_struct( self );
  return DBL2NUM( worker__predict_productivity( worker, NUM2INT( rv_start ), NUM2INT( rv_size ) ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_class_worker() {
  worker__init_cache();

  // Worker instantiation and class methods
  rb_define_alloc_func( FactoryElf_Worker, worker_alloc );
  rb_define_method( FactoryElf_Worker, "initialize", worker_class_initialize, 2 );
  rb_define_method( FactoryElf_Worker, "initialize_copy", worker_class_initialize_copy, 1 );

  // Worker attributes
  rb_define_method( FactoryElf_Worker, "id", worker_object_id, 0 );
  rb_define_method( FactoryElf_Worker, "productivity", worker_object_productivity, 0 );
  rb_define_method( FactoryElf_Worker, "ready_abs_mins", worker_object_ready_abs_mins, 0 );

  // Worker methods
  rb_define_method( FactoryElf_Worker, "calc_new_productivity", worker_object_calc_new_productivity, 2 );
  rb_define_method( FactoryElf_Worker, "calc_build_time_mins", worker_object_build_time_mins, 1 );
  rb_define_method( FactoryElf_Worker, "skip_rest_of_day", worker_object_skip_rest_of_day, 0 );
  rb_define_method( FactoryElf_Worker, "update_for_toy_build", worker_object_update_for_toy_build, 2 );
  rb_define_method( FactoryElf_Worker, "predict_productivity", worker_object_predict_productivity, 2 );
  rb_define_method( FactoryElf_Worker, "max_sanctioned_size", worker_object_max_sanctioned_size, 0 );
  rb_define_method( FactoryElf_Worker, "max_no_loss_size", worker_object_max_no_loss_size, 0 );
  rb_define_method( FactoryElf_Worker, "best_training_size", worker_object_best_training_size, 1 );

}
