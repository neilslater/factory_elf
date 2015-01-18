// ext/factory_elf/struct_office.c

#include "struct_office.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating Office structs
//

Office *office__create() {
  Office *office;
  office = xmalloc( sizeof(Office) );

  office->narr_toys = Qnil;
  office->narr_toys_size_idx = Qnil;

  office->toys_ptr = NULL;
  office->toys_size_idx_ptr = NULL;

  office->index_max = 9999999;

  return office;
}

void office__init_arrays( Office *office ) {
  int *ptr,i;
  struct NARRAY *narr;

  // Attach TOYS array to narr_toys
  office->narr_toys = rb_const_get( FactoryElf, rb_intern("TOYS") );
  GetNArray( office->narr_toys, narr );
  office->toys_ptr = (int*) narr->ptr;

  // Attach TOYS_SIZE_IDX array to narr_toys_size_id
  office->narr_toys_size_idx = rb_const_get( FactoryElf, rb_intern("TOYS_SIZE_IDX") );
  GetNArray( office->narr_toys_size_idx, narr );

  // This is a copy, so it can be compressed later
  ptr = (int*) narr->ptr;
  office->toys_size_idx_ptr = ALLOC_N( int, 10000000 );
  for(i=0;i<10000000;i++) {
    office->toys_size_idx_ptr[i] = ptr[i];
  }

  return;
}

void office__gc_mark( Office *office ) {
  rb_gc_mark( office->narr_toys );
  rb_gc_mark( office->narr_toys_size_idx );
}

void office__destroy( Office *office ) {
  xfree( office );
  // No need to free NArrays - they will be handled by Ruby's GC, and may still be reachable
  return;
}

int office__find_toy( Office *office, Schedule *schedule, int min_id, int max_abs_mins, int max_size, int min_size ) {
  int *schedule_ptr = schedule->schedule_ptr;
  int *toys_ptr = office->toys_ptr;
  int *size_idx_ptr = office->toys_size_idx_ptr;

  int duration_offset = 1;
  int abs_mins_offset = 0;
  Toy this_toy, next_toy;
  int found_toy_id = -1;

  int size_idx_lower = 0;
  int size_idx_upper = office->index_max;
  int size_idx_current = office->index_max/2;

  if ( max_size < 1 || min_size > 47473 || min_size > max_size ) {
    return -1;
  }

  // Very simple binary search to find toy with best matching size
  while( size_idx_lower < size_idx_upper ) {
    toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ] );
    toy__read_item( &next_toy, toys_ptr, size_idx_ptr[ size_idx_current + 1 ] );

    if ( this_toy.size <= max_size && next_toy.size > max_size ) {
      break;
    }

    if ( next_toy.size <= max_size ) {
      size_idx_lower = size_idx_current;
      size_idx_current = ( size_idx_current + size_idx_upper + 1 ) / 2;
      if ( size_idx_current >= office->index_max ) {
        size_idx_current = office->index_max;
        break;
      }
      continue;
    }

    size_idx_upper = size_idx_current;
    size_idx_current = ( size_idx_current + size_idx_lower ) / 2;
  }

  if ( min_id < 1 && max_abs_mins > 500000 ) {
    // We can ignore constraints on id and schedule time, and do less checks
    while ( size_idx_current >= 0 ) {
      toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ] );

      if ( this_toy.size < min_size ) {
        break;
      }
      if ( !schedule_ptr[ this_toy.toy_id * 3 + 1 ] ) {
        found_toy_id = this_toy.toy_id;
        break;
      }
      size_idx_current--;
    }
  } else {
    while ( size_idx_current >= 0 ) {
      toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ]);

      if ( this_toy.size < min_size ) {
        break;
      }
      if ( !schedule_ptr[ this_toy.toy_id * 3 + 1 ] && min_id <= this_toy.toy_id && max_abs_mins >= this_toy.abs_mins ) {
        found_toy_id = this_toy.toy_id;
        break;
      }
      size_idx_current--;
    }
  }

  return found_toy_id;
}


void office__compress_size_index( Office *office, Schedule *schedule ) {
  int *schedule_ptr = schedule->schedule_ptr;
  int *size_idx_ptr = office->toys_size_idx_ptr;
  int i,toy_id;
  int idx_pos = 0;

  for( i=0; i <= office->index_max; i++ ) {
    toy_id = size_idx_ptr[i];
    if ( !schedule_ptr[ toy_id * 3 + 1 ] ) {
      size_idx_ptr[idx_pos] = toy_id;
      idx_pos++;
    }
  }
  office->index_max = idx_pos;
  return;
}


int office__count_toys_between( Office *office, Schedule *schedule, int min_size, int max_size ) {
  int *schedule_ptr = schedule->schedule_ptr;
  int *toys_ptr = office->toys_ptr;
  int *size_idx_ptr = office->toys_size_idx_ptr;

  int duration_offset = 1;
  int abs_mins_offset = 0;
  Toy this_toy, next_toy;
  int found_toy_count = 0;

  int size_idx_lower = 0;
  int size_idx_upper = office->index_max;
  int size_idx_current = office->index_max/2;

  if ( max_size < 1 || min_size > 47473 || min_size > max_size ) {
    return 0;
  }

  // Very simple binary search to find toy with size <= max_size
  while( size_idx_lower < size_idx_upper ) {
    toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ] );
    toy__read_item( &next_toy, toys_ptr, size_idx_ptr[ size_idx_current + 1 ] );

    if ( this_toy.size <= max_size && next_toy.size > max_size ) {
      break;
    }

    if ( next_toy.size <= max_size ) {
      size_idx_lower = size_idx_current;
      size_idx_current = ( size_idx_current + size_idx_upper + 1 ) / 2;
      if ( size_idx_current >= office->index_max ) {
        size_idx_current = office->index_max;
        break;
      }
      continue;
    }

    size_idx_upper = size_idx_current;
    size_idx_current = ( size_idx_current + size_idx_lower ) / 2;
  }

  while ( size_idx_current >= 0 ) {
    toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ]);

    size_idx_current--;

    // Don't count used toys
    if ( schedule_ptr[ this_toy.toy_id * 3 + 1 ] ) {
      continue;
    }
    // Stop once we are below min size in size index
    if ( this_toy.size < min_size ) {
      break;
    }
    found_toy_count++;
  }

  return found_toy_count;
}


long office__sum_toy_sizes_between( Office *office, Schedule *schedule, int min_size, int max_size ) {
  int *schedule_ptr = schedule->schedule_ptr;
  int *toys_ptr = office->toys_ptr;
  int *size_idx_ptr = office->toys_size_idx_ptr;

  int duration_offset = 1;
  int abs_mins_offset = 0;
  Toy this_toy, next_toy;
  long total_size = 0;

  int size_idx_lower = 0;
  int size_idx_upper = office->index_max;
  int size_idx_current = office->index_max/2;

  if ( max_size < 1 || min_size > 47473 || min_size > max_size ) {
    return 0;
  }

  // Very simple binary search to find toy with size <= max_size
  while( size_idx_lower < size_idx_upper ) {
    toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ] );
    toy__read_item( &next_toy, toys_ptr, size_idx_ptr[ size_idx_current + 1 ] );

    if ( this_toy.size <= max_size && next_toy.size > max_size ) {
      break;
    }

    if ( next_toy.size <= max_size ) {
      size_idx_lower = size_idx_current;
      size_idx_current = ( size_idx_current + size_idx_upper + 1 ) / 2;
      if ( size_idx_current >= office->index_max ) {
        size_idx_current = office->index_max;
        break;
      }
      continue;
    }

    size_idx_upper = size_idx_current;
    size_idx_current = ( size_idx_current + size_idx_lower ) / 2;
  }

  while ( size_idx_current >= 0 ) {
    toy__read_item( &this_toy, toys_ptr, size_idx_ptr[ size_idx_current ]);

    size_idx_current--;

    // Don't count used toys
    if ( schedule_ptr[ this_toy.toy_id * 3 + 1 ] ) {
      continue;
    }
    // Stop once we are below min size in size index
    if ( this_toy.size < min_size ) {
      break;
    }
    total_size += (long) this_toy.size;
  }

  return total_size;
}
