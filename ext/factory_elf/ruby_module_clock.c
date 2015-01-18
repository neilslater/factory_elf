// ext/factory_elf/rub_module_clock.c

#include "ruby_module_clock.h"

/* @overload from_yymmdd_hhmm( yy, mm, dd, hh, mins )
 * Convert from year, month, day, hour, mins to absolute mins since midnight, 1 Jan 2014
 * @param [Integer] yy year, e.g. 2014
 * @param [Integer] mm month, e.g. 2 for February
 * @param [Integer] dd day of month, e.g. 17
 * @param [Integer] hh hour of day, e.g. 23
 * @param [Integer] mins minute, e.g. 59
 * @return [Integer] number of minutes
 */
static VALUE ruby_module_from_yymmdd_hhmm( VALUE self, VALUE yy, VALUE mm, VALUE dd, VALUE hh, VALUE mins ) {
  return INT2NUM(
    clock__from_yymmdd_hhmm( NUM2INT(yy), NUM2INT(mm), NUM2INT(dd), NUM2INT(hh), NUM2INT(mins) )
  );
}

/* @overload mins_to_yymmdd_hhmm( abs_mins )
 * Convert from absolute mins to [year, month, day, hour, mins]
 * @param [Integer] abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Array] in form [year, month, day, hour, mins]
 */
static VALUE ruby_module_mins_to_yymmdd_hhmm( VALUE self, VALUE rv_abs_mins ) {
  int abs_mins = NUM2INT( rv_abs_mins );
  volatile VALUE dt = rb_ary_new2( 5 );

  rb_ary_store( dt, 0, INT2NUM( clock__yy( abs_mins ) ));
  rb_ary_store( dt, 1, INT2NUM( clock__mm( abs_mins ) ));
  rb_ary_store( dt, 2, INT2NUM( clock__dd( abs_mins ) ));
  rb_ary_store( dt, 3, INT2NUM( clock__hh( abs_mins ) ));
  rb_ary_store( dt, 4, INT2NUM( clock__mins( abs_mins ) ));

  return dt;
}

/* @overload sanctioned_minute?( abs_mins )
 * Convert from absolute mins to [year, month, day, hour, mins]
 * @param [Integer] abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Boolean] true if whole minute is in sanctioned time
 */
static VALUE ruby_module_in_sanctioned_time( VALUE self, VALUE rv_abs_mins ) {
  if ( clock__in_sanctioned_time( NUM2INT( rv_abs_mins ) ) ) {
    return Qtrue;
  }
  return Qfalse;
}

/* @overload next_sanctioned_start( abs_mins )
 * Absolute minute of the start of the next sanctioned time. Always skips to the next
 * start.
 * @param [Integer] abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Integer] abs_mins value for next start point of a sanctioned time period
 */
static VALUE ruby_module_next_sanctioned_start( VALUE self, VALUE rv_abs_mins ) {
  return INT2NUM( clock__next_sanctioned_start( NUM2INT( rv_abs_mins ) ) );
}

/* @overload next_sanctioned_end( abs_mins )
 * Absolute minute of the end of the next sanctioned time. Always skips to the next
 * end.
 * @param [Integer] abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Integer] abs_mins value for next end point of a sanctioned time period
 */
static VALUE ruby_module_next_sanctioned_end( VALUE self, VALUE rv_abs_mins ) {
  return INT2NUM( clock__next_sanctioned_end( NUM2INT( rv_abs_mins ) ) );
}

/* @overload sanctioned_minutes_between( start_abs_mins, end_abs_minutes )
 * Number of sanctioned minutes in inclusive range (start_abs_mins..end_abs_minutes) - i.e. if the two
 * values are the same, then that counts 1 minute if in sanctioned time period.
 * @param [Integer] start_abs_mins number of minutes since midnight, 1 Jan 2014
 * @param [Integer] end_abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Integer] number of minutes to next end
 */
static VALUE ruby_module_sanctioned_minutes_between( VALUE self, VALUE rv_start_mins, VALUE rv_end_mins ) {
  return INT2NUM( clock__sanctioned_minutes_between( NUM2INT( rv_start_mins ), NUM2INT( rv_end_mins ) ) );
}

/* @overload non_sanctioned_minutes_between( start_abs_mins, end_abs_minutes )
 * Number of non_sanctioned minutes in inclusive range (start_abs_mins..end_abs_minutes) - i.e. if the two
 * values are the same, then that counts 1 minute if in non_sanctioned time period.
 * @param [Integer] start_abs_mins number of minutes since midnight, 1 Jan 2014
 * @param [Integer] end_abs_mins number of minutes since midnight, 1 Jan 2014
 * @return [Integer] number of minutes to next end
 */
static VALUE ruby_module_non_sanctioned_minutes_between( VALUE self, VALUE rv_start_mins, VALUE rv_end_mins ) {
  return INT2NUM( clock__non_sanctioned_minutes_between( NUM2INT( rv_start_mins ), NUM2INT( rv_end_mins ) ) );
}

/* @overload add_work_minutes( start_abs_mins, duration )
 * Returns next available time if a period of work starts at given time. The new time should always
 * be a sanctioned minute.
 * @param [Integer] start_abs_mins number of minutes since midnight, 1 Jan 2014
 * @param [Integer] duration length of work to do, in minutes
 * @return [Integer] abs_min when next piece of work could start
 */
static VALUE ruby_module_add_work_minutes( VALUE self, VALUE rv_start_mins, VALUE rv_duration ) {
  return INT2NUM( clock__add_work_minutes( NUM2INT( rv_start_mins ), NUM2INT( rv_duration ) ) );
}

static VALUE ruby_module_sanctioned_minutes_remaining( VALUE self, VALUE rv_start_mins ) {
  return INT2NUM( clock__sanctioned_minutes_remaining( NUM2INT( rv_start_mins ) ) );
}


void init_module_clock() {
  rb_define_singleton_method( FactoryElf_Clock, "from_yymmdd_hhmm", ruby_module_from_yymmdd_hhmm, 5 );
  rb_define_singleton_method( FactoryElf_Clock, "mins_to_yymmdd_hhmm", ruby_module_mins_to_yymmdd_hhmm, 1 );
  rb_define_singleton_method( FactoryElf_Clock, "sanctioned_minute?", ruby_module_in_sanctioned_time, 1 );
  rb_define_singleton_method( FactoryElf_Clock, "next_sanctioned_start", ruby_module_next_sanctioned_start, 1 );
  rb_define_singleton_method( FactoryElf_Clock, "next_sanctioned_end", ruby_module_next_sanctioned_end, 1 );
  rb_define_singleton_method( FactoryElf_Clock, "sanctioned_minutes_between", ruby_module_sanctioned_minutes_between, 2 );
  rb_define_singleton_method( FactoryElf_Clock, "non_sanctioned_minutes_between", ruby_module_non_sanctioned_minutes_between, 2 );
  rb_define_singleton_method( FactoryElf_Clock, "add_work_minutes", ruby_module_add_work_minutes, 2 );
  rb_define_singleton_method( FactoryElf_Clock, "sanctioned_minutes_remaining", ruby_module_sanctioned_minutes_remaining, 1 );
}
