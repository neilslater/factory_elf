// ext/factory_elf/ruby_class_schedule_item.c

#include "ruby_class_schedule_item.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE schedule_item_as_ruby_class( ScheduleItem *schedule_item , VALUE klass ) {
  return Data_Wrap_Struct( klass, NULL, schedule_item__destroy, schedule_item );
}

VALUE schedule_item_alloc(VALUE klass) {
  return schedule_item_as_ruby_class( schedule_item__create(), klass );
}

inline ScheduleItem *get_schedule_item_struct( VALUE obj ) {
  ScheduleItem *schedule_item;
  Data_Get_Struct( obj, ScheduleItem, schedule_item );
  return schedule_item;
}

void assert_value_wraps_schedule_item( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)schedule_item__destroy) {
    rb_raise( rb_eTypeError, "Expected a FactoryElf_ScheduleItem object, but got something else" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VALUE schedule_item_object_initialize( VALUE self, VALUE rv_toy_id, VALUE rv_elf_id, VALUE rv_abs_mins, VALUE rv_duration ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );

  int new_val = NUM2INT( rv_toy_id );
  if ( new_val < 0 || new_val > 9999999 ) {
    rb_raise( rb_eArgError, "Toy id %d out of bounds", new_val );
  }
  schedule_item->toy_id = new_val;

  new_val = NUM2INT( rv_elf_id );
  if ( new_val < 0 || new_val > 899 ) {
    rb_raise( rb_eArgError, "Elf id %d out of bounds", new_val );
  }
  schedule_item->elf_id = new_val;

  new_val = NUM2INT( rv_abs_mins );
  if ( new_val < 540 ) {
    rb_raise( rb_eArgError, "Abs mins %d out of bounds", new_val );
  }
  schedule_item->abs_mins = new_val;

  new_val = NUM2INT( rv_duration );
  if ( new_val < 1 ) {
    rb_raise( rb_eArgError, "Duration %d out of bounds", new_val );
  }
  schedule_item->duration = new_val;

  return self;
}

VALUE schedule_item_object_toy_id( VALUE self ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  return INT2FIX( schedule_item->toy_id );
}

VALUE schedule_item_object_elf_id( VALUE self ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  return INT2FIX( schedule_item->elf_id );
}

VALUE schedule_item_object_abs_mins( VALUE self ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  return INT2FIX( schedule_item->abs_mins );
}

VALUE schedule_item_object_duration( VALUE self ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  return INT2FIX( schedule_item->duration );
}

VALUE schedule_item_object_toy_id_set( VALUE self, VALUE rv  ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  int new_val = NUM2INT( rv );
  if ( new_val < 0 || new_val > 9999999 ) {
    rb_raise( rb_eArgError, "Toy id %d out of bounds", new_val );
  }
  schedule_item->toy_id = new_val;
  return INT2FIX( schedule_item->toy_id );
}

VALUE schedule_item_object_elf_id_set( VALUE self, VALUE rv  ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  int new_val = NUM2INT( rv );
  if ( new_val < 0 || new_val > 899 ) {
    rb_raise( rb_eArgError, "Elf id %d out of bounds", new_val );
  }
  schedule_item->elf_id = new_val;
  return INT2FIX( schedule_item->elf_id );
}

VALUE schedule_item_object_abs_mins_set( VALUE self, VALUE rv ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  int new_val = NUM2INT( rv );
  if ( new_val < 0 ) {
    rb_raise( rb_eArgError, "Abs mins %d out of bounds", new_val );
  }
  schedule_item->abs_mins = new_val;
  return INT2FIX( schedule_item->abs_mins );
}

VALUE schedule_item_object_duration_set( VALUE self, VALUE rv ) {
  ScheduleItem *schedule_item = get_schedule_item_struct( self );
  int new_val = NUM2INT( rv );
  if ( new_val < 1 || new_val > 5000000 ) {
    rb_raise( rb_eArgError, "Duration %d out of bounds", new_val );
  }
  schedule_item->duration = new_val;
  return INT2FIX( schedule_item->duration );
}

void init_class_schedule_item() {
  rb_define_alloc_func( FactoryElf_ScheduleItem, schedule_item_alloc );
  rb_define_method( FactoryElf_ScheduleItem, "initialize", schedule_item_object_initialize, 4 );

  rb_define_method( FactoryElf_ScheduleItem, "toy_id", schedule_item_object_toy_id, 0 );
  rb_define_method( FactoryElf_ScheduleItem, "elf_id", schedule_item_object_elf_id, 0 );
  rb_define_method( FactoryElf_ScheduleItem, "abs_mins", schedule_item_object_abs_mins, 0 );
  rb_define_method( FactoryElf_ScheduleItem, "duration", schedule_item_object_duration, 0 );

  rb_define_method( FactoryElf_ScheduleItem, "toy_id=", schedule_item_object_toy_id_set, 1 );
  rb_define_method( FactoryElf_ScheduleItem, "elf_id=", schedule_item_object_elf_id_set, 1 );
  rb_define_method( FactoryElf_ScheduleItem, "abs_mins=", schedule_item_object_abs_mins_set, 1 );
  rb_define_method( FactoryElf_ScheduleItem, "duration=", schedule_item_object_duration_set, 1 );

  return;
}
