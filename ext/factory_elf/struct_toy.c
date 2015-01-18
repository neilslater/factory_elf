// ext/factory_elf/struct_toy.c

#include "struct_toy.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating Toy structs
//

Toy *toy__create() {
  Toy *toy;
  toy = xmalloc( sizeof(Toy) );

  toy->toy_id = 0;
  toy->abs_mins = 0;
  toy->size = 0;

  return toy;
}

void toy__destroy( Toy *toy ) {
  xfree( toy );
  // No need to free NArrays - they will be handled by Ruby's GC, and may still be reachable
  return;
}

void toy__read_item( Toy *toy, int *ptr, int id ) {
  toy->toy_id = id;
  toy->abs_mins = ptr[ id * 2 ];
  toy->size = ptr[ id * 2 + 1 ];
  return;
}
