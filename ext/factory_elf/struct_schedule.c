// ext/factory_elf/struct_schedule.c

#include "struct_schedule.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating Schedule structs
//

static int schedule_shape[2] = { 3, 10000000 };

Schedule *schedule__create() {
  Schedule *schedule;
  schedule = xmalloc( sizeof(Schedule) );

  schedule->narr_schedule = Qnil;
  schedule->schedule_ptr = NULL;

  return schedule;
}

void schedule__init_arrays( Schedule *schedule ) {
  int *ptr,i;
  struct NARRAY *narr;

  schedule->narr_schedule = na_make_object( NA_LINT, 2, schedule_shape, cNArray );
  GetNArray( schedule->narr_schedule, narr );
  ptr = (int*) narr->ptr;
  for( i=0; i<30000000; i++ ) { ptr[i] = 0; }
  schedule->schedule_ptr = ptr;

  return;
}

void schedule__destroy( Schedule *schedule ) {
  xfree( schedule );
  // No need to free NArrays - they will be handled by Ruby's GC, and may still be reachable
  return;
}

// Called by Ruby's GC, we have to mark all child objects that could be in Ruby
// space, so that they don't get deleted.
void schedule__gc_mark( Schedule *schedule ) {
  rb_gc_mark( schedule->narr_schedule );
  return;
}

int schedule__toy_planned( Schedule *schedule, int toy_id ) {
  return ( schedule->schedule_ptr[ toy_id * 3 + 1 ] ? 1 : 0 );
}

ScheduleItem *schedule_item__create() {
  ScheduleItem *schedule_item;
  schedule_item = xmalloc( sizeof(ScheduleItem) );

  schedule_item->toy_id = 0;
  schedule_item->elf_id = 0;
  schedule_item->abs_mins = 0;
  schedule_item->duration = 0;

  return schedule_item;
}

void schedule_item__destroy( ScheduleItem *schedule_item ) {
  xfree( schedule_item );
  // No need to free NArrays - they will be handled by Ruby's GC, and may still be reachable
  return;
}

void schedule__read_item( Schedule *schedule, ScheduleItem *si ) {
  int toy_offset = si->toy_id * 3;
  si->elf_id = schedule->schedule_ptr[ toy_offset ];
  si->abs_mins = schedule->schedule_ptr[ toy_offset + 1 ];
  si->duration = schedule->schedule_ptr[ toy_offset + 2 ];
  return;
}

void schedule__write_item( Schedule *schedule, ScheduleItem *si ) {
  int toy_offset = si->toy_id * 3;
  schedule->schedule_ptr[ toy_offset ] = si->elf_id;
  schedule->schedule_ptr[ toy_offset + 1 ] = si->abs_mins;
  schedule->schedule_ptr[ toy_offset + 2 ] = si->duration;
  return;
}
