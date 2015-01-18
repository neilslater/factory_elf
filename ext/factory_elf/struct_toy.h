// ext/factory_elf/struct_toy.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Toy structs
//

#ifndef STRUCT_TOY_H
#define STRUCT_TOY_H

#include <ruby.h>
#include "narray.h"

typedef struct _toy_raw {
    int toy_id;
    int abs_mins;
    int size;
  } Toy;

Toy *toy__create();

void toy__destroy( Toy *toy );

void toy__read_item( Toy *toy, int *ptr, int id );

#endif
