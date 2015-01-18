// ext/factory_elf/struct_schedule.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Schedule structs
//

#ifndef STRUCT_SCHEDULE_H
#define STRUCT_SCHEDULE_H

#include <ruby.h>
#include "narray.h"
#include "shared_vars.h"

typedef struct _schedule_raw {
    volatile VALUE narr_schedule;
    int * schedule_ptr;
  } Schedule;

typedef struct _schedule_item_raw {
    int toy_id;
    int elf_id;
    int abs_mins;
    int duration;
  } ScheduleItem;

Schedule *schedule__create();

void schedule__init_arrays( Schedule *schedule );

void schedule__destroy( Schedule *schedule );

void schedule__gc_mark( Schedule *schedule );

ScheduleItem *schedule_item__create();

void schedule_item__destroy( ScheduleItem *schedule_item );

/////////////////////////////

int schedule__toy_planned( Schedule *schedule, int toy_id );

void schedule__read_item( Schedule *schedule, ScheduleItem *si );

void schedule__write_item( Schedule *schedule, ScheduleItem *si );

#endif
