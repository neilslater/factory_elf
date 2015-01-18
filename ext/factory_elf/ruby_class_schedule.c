// ext/factory_elf/ruby_class_schedule.c

#include "ruby_class_schedule.h"
#include "ruby_class_schedule_item.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE schedule_as_ruby_class( Schedule *schedule , VALUE klass ) {
  return Data_Wrap_Struct( klass, schedule__gc_mark, schedule__destroy, schedule );
}

VALUE schedule_alloc(VALUE klass) {
  return schedule_as_ruby_class( schedule__create(), klass );
}

inline Schedule *get_schedule_struct( VALUE obj ) {
  Schedule *schedule;
  Data_Get_Struct( obj, Schedule, schedule );
  return schedule;
}

void assert_value_wraps_schedule( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)schedule__destroy) {
    rb_raise( rb_eTypeError, "Expected a FactoryElf_Schedule object, but got something else" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VALUE schedule_object_initialize( VALUE self ) {
  Schedule *schedule = get_schedule_struct( self );
  schedule__init_arrays( schedule );
  return self;
}

VALUE schedule_class_from_narray( VALUE self, VALUE rv_narr_schedule ) {
  Schedule *schedule = schedule__create();
  struct NARRAY *narr;
  GetNArray( rv_narr_schedule, narr );

  // TODO: check type and shape is correct

  schedule->narr_schedule = rv_narr_schedule;
  schedule->schedule_ptr = (int*) narr->ptr;

  return schedule_as_ruby_class( schedule, self );
}

VALUE schedule_object_data( VALUE self ) {
  Schedule *schedule = get_schedule_struct( self );
  return schedule->narr_schedule;
}

VALUE schedule_object_read_item( VALUE self, VALUE rv_i ) {
  Schedule *schedule = get_schedule_struct( self );
  ScheduleItem *schedule_item = schedule_item__create();
  volatile VALUE rv_si = Data_Wrap_Struct( FactoryElf_ScheduleItem, NULL, schedule_item__destroy, schedule_item );

  int toy_id = NUM2INT( rv_i );
  if ( toy_id < 0 || toy_id > 9999999 ) {
    rb_raise( rb_eArgError, "Toy id %d out of bounds", toy_id );
  }
  schedule_item->toy_id = toy_id;
  schedule__read_item( schedule, schedule_item );

  return rv_si;
}

VALUE schedule_object_write_item( VALUE self, VALUE rv_si ) {
  Schedule *schedule = get_schedule_struct( self );
  ScheduleItem *schedule_item;
  assert_value_wraps_schedule_item( rv_si ); // Protects against weird errors
  Data_Get_Struct( rv_si, ScheduleItem, schedule_item );

  schedule__write_item( schedule, schedule_item );
  return self;
}


void init_class_schedule( ) {
  rb_define_alloc_func( FactoryElf_Schedule, schedule_alloc );
  rb_define_method( FactoryElf_Schedule, "initialize", schedule_object_initialize, 0 );

  rb_define_method( FactoryElf_Schedule, "data", schedule_object_data, 0 );

  rb_define_method( FactoryElf_Schedule, "fetch_item", schedule_object_read_item, 1 );
  rb_define_method( FactoryElf_Schedule, "store_item", schedule_object_write_item, 1 );

  rb_define_singleton_method( FactoryElf_Schedule, "from_narray", schedule_class_from_narray, 1 );

  return;
}
