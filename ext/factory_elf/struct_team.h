// ext/factory_elf/struct_team.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Declarations of OO-style functions for manipulating Team structs
//

#ifndef STRUCT_TEAM_H
#define STRUCT_TEAM_H

#include <ruby.h>
#include "narray.h"
#include "struct_worker.h"

typedef struct _team_raw {
    int first_worker;
    int * next_worker;
    Worker * workers;
  } Team;

Team *team__create();

void team__init_arrays( Team *team, int start_abs_mins );

Worker *team__first_worker( Team *team );

void team__sort_first_worker( Team *team );

void team__destroy( Team *team );

#endif
