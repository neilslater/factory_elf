// ext/factory_elf/ruby_class_toy.c

#include "ruby_class_toy.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE toy_as_ruby_class( Toy *toy , VALUE klass ) {
  return Data_Wrap_Struct( klass, NULL, toy__destroy, toy );
}

VALUE toy_alloc(VALUE klass) {
  return toy_as_ruby_class( toy__create(), klass );
}

inline Toy *get_toy_struct( VALUE obj ) {
  Toy *toy;
  Data_Get_Struct( obj, Toy, toy );
  return toy;
}

void assert_value_wraps_toy( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)toy__destroy) {
    rb_raise( rb_eTypeError, "Expected a FactoryElf_Toy object, but got something else" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VALUE toy_object_initialize( VALUE self, VALUE rv_toy_id, VALUE rv_abs_mins, VALUE rv_size ) {
  Toy *toy = get_toy_struct( self );

  int new_val = NUM2INT( rv_toy_id );
  if ( new_val < 0 || new_val > 9999999 ) {
    rb_raise( rb_eArgError, "Toy id %d out of bounds", new_val );
  }
  toy->toy_id = new_val;

  new_val = NUM2INT( rv_abs_mins );
  if ( new_val < 540 ) {
    rb_raise( rb_eArgError, "Abs mins %d out of bounds", new_val );
  }
  toy->abs_mins = new_val;

  new_val = NUM2INT( rv_size );
  if ( new_val < 1 ) {
    rb_raise( rb_eArgError, "Size %d out of bounds", new_val );
  }
  toy->size = new_val;

  return self;
}

VALUE toy_object_toy_id( VALUE self ) {
  Toy *toy = get_toy_struct( self );
  return INT2FIX( toy->toy_id );
}

VALUE toy_object_abs_mins( VALUE self ) {
  Toy *toy = get_toy_struct( self );
  return INT2FIX( toy->abs_mins );
}

VALUE toy_object_size( VALUE self ) {
  Toy *toy = get_toy_struct( self );
  return INT2FIX( toy->size );
}

VALUE toy_class_toy_fetch( VALUE self, VALUE rv_toy_id ) {
  int id = NUM2INT( rv_toy_id );
  struct NARRAY *narr;

  if ( id < 0 || id > 9999999 ) {
    rb_raise( rb_eArgError, "Toy id %d out of bounds", id );
  }

  GetNArray( rb_const_get( FactoryElf, rb_intern("TOYS") ), narr );

  volatile VALUE new_toy = toy_alloc( FactoryElf_Toy );
  Toy *toy = get_toy_struct( new_toy );

  toy__read_item( toy, (int*) narr->ptr, id );

  return new_toy;
}

void init_class_toy() {
  rb_define_alloc_func( FactoryElf_Toy, toy_alloc );
  rb_define_method( FactoryElf_Toy, "initialize", toy_object_initialize, 3 );

  rb_define_method( FactoryElf_Toy, "toy_id", toy_object_toy_id, 0 );
  rb_define_method( FactoryElf_Toy, "abs_mins", toy_object_abs_mins, 0 );
  rb_define_method( FactoryElf_Toy, "size", toy_object_size, 0 );

  rb_define_singleton_method( FactoryElf_Toy, "[]", toy_class_toy_fetch, 1 );

  return;
}
