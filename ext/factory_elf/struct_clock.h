// ext/factory_elf/struct_clock.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Clock structs
//

#ifndef STRUCT_CLOCK_H
#define STRUCT_CLOCK_H

#include <ruby.h>

// This isn't a struct, but based on abs_minutes starting at 0 on 2014-01-01T00:00:00

int clock__yy( int abs_minutes );
int clock__mm( int abs_minutes );
int clock__dd( int abs_minutes );
int clock__hh( int abs_minutes );
int clock__mins( int abs_minutes );

int clock__from_yymmdd_hhmm( int y, int m, int d, int h, int mins );

int clock__in_sanctioned_time( int abs_minutes );
int clock__next_sanctioned_start( int abs_minutes );
int clock__next_sanctioned_end( int abs_minutes );
int clock__sanctioned_minutes_remaining( int start_abs_minutes );
int clock__sanctioned_minutes_between( int start_abs_minutes, int end_abs_minutes );
int clock__non_sanctioned_minutes_between( int start_abs_minutes, int end_abs_minutes );
int clock__add_work_minutes( int abs_minutes, int work_minutes );

#endif
