// ext/factory_elf/ruby_class_office.c

#include "ruby_class_office.h"
#include "ruby_class_toy.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE office_as_ruby_class( Office *office , VALUE klass ) {
  return Data_Wrap_Struct( klass, office__gc_mark, office__destroy, office );
}

VALUE office_alloc(VALUE klass) {
  return office_as_ruby_class( office__create(), klass );
}

inline Office *get_office_struct( VALUE obj ) {
  Office *office;
  Data_Get_Struct( obj, Office, office );
  return office;
}

void assert_value_wraps_office( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)office__destroy) {
    rb_raise( rb_eTypeError, "Expected a FactoryElf_Office object, but got something else" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VALUE office_object_initialize( VALUE self ) {
  Office *office = get_office_struct( self );
  office__init_arrays( office );
  return self;
}

VALUE office_object_toys( VALUE self ) {
  Office *office = get_office_struct( self );
  return office->narr_toys;
}

VALUE office_object_toys_size_idx( VALUE self ) {
  Office *office = get_office_struct( self );
  return office->narr_toys_size_idx;
}

VALUE office_object_find_toy( VALUE self, VALUE rv_schedule, VALUE rv_min_id, VALUE rv_max_abs_mins, VALUE rv_max_size, VALUE rv_min_size ) {
  Office *office = get_office_struct( self );
  Schedule *schedule;
  Data_Get_Struct( rv_schedule, Schedule, schedule );

  int toy_id = office__find_toy( office, schedule, NUM2INT(rv_min_id), NUM2INT(rv_max_abs_mins), NUM2INT(rv_max_size), NUM2INT(rv_min_size) );

  if ( toy_id < 0 || toy_id > 9999999 ) {
    return Qnil;
  }

  Toy *toy = toy__create();
  toy__read_item( toy, office->toys_ptr, toy_id );

  return Data_Wrap_Struct( FactoryElf_Toy, NULL, toy__destroy, toy );
}

VALUE office_object_compress_size_index( VALUE self, VALUE rv_schedule ) {
  Office *office = get_office_struct( self );
  Schedule *schedule;
  Data_Get_Struct( rv_schedule, Schedule, schedule );

  office__compress_size_index( office, schedule );

  return self;
}

VALUE office_object_schedule_toy_build( VALUE self, VALUE rv_schedule, VALUE rv_team, VALUE rv_toy, VALUE rv_start_time ) {
  Office *office = get_office_struct( self );
  Schedule *schedule;
  Data_Get_Struct( rv_schedule, Schedule, schedule );
  Team *team;
  Data_Get_Struct( rv_team, Team, team );
  Toy *toy;
  Data_Get_Struct( rv_toy, Toy, toy );
  int start_abs_mins = NUM2INT( rv_start_time );

  Worker *worker = team__first_worker( team );

  int build_time_mins = worker__calc_build_time_mins( worker, toy->size );

  worker__update_for_toy_build( worker, start_abs_mins, toy->size );

  ScheduleItem * si = schedule_item__create();
  si->toy_id = toy->toy_id;
  si->elf_id = worker->id;
  si->abs_mins = start_abs_mins;
  si->duration = build_time_mins;
  schedule__write_item( schedule, si );

  return Data_Wrap_Struct( FactoryElf_ScheduleItem, NULL, schedule_item__destroy, si );
}

VALUE office_object_count_toys_between( VALUE self, VALUE rv_schedule, VALUE rv_min_size, VALUE rv_max_size ) {
  Office *office = get_office_struct( self );
  Schedule *schedule;
  Data_Get_Struct( rv_schedule, Schedule, schedule );

  return INT2NUM( office__count_toys_between( office, schedule, NUM2INT(rv_min_size), NUM2INT(rv_max_size) ) );
}

VALUE office_object_sum_toy_sizes_between( VALUE self, VALUE rv_schedule, VALUE rv_min_size, VALUE rv_max_size ) {
  Office *office = get_office_struct( self );
  Schedule *schedule;
  Data_Get_Struct( rv_schedule, Schedule, schedule );

  return LONG2NUM( office__sum_toy_sizes_between( office, schedule, NUM2INT(rv_min_size), NUM2INT(rv_max_size) ) );
}

void init_class_office() {
  rb_define_alloc_func( FactoryElf_Office, office_alloc );
  rb_define_method( FactoryElf_Office, "initialize", office_object_initialize, 0 );
  rb_define_method( FactoryElf_Office, "toys", office_object_toys, 0 );
  rb_define_method( FactoryElf_Office, "toys_size_idx", office_object_toys_size_idx, 0 );
  rb_define_method( FactoryElf_Office, "find_toy", office_object_find_toy, 5 );
  rb_define_method( FactoryElf_Office, "count_toys_between", office_object_count_toys_between, 3 );
  rb_define_method( FactoryElf_Office, "sum_toy_sizes_between", office_object_sum_toy_sizes_between, 3 );
  rb_define_method( FactoryElf_Office, "compress_size_index", office_object_compress_size_index, 1 );
  rb_define_method( FactoryElf_Office, "schedule_toy_build", office_object_schedule_toy_build, 4 );
  return;
}
