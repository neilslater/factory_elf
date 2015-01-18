// ext/factory_elf/struct_team.c

#include "struct_team.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions of OO-style functions for manipulating Team structs
//

Team *team__create() {
  Team *team;
  team = xmalloc( sizeof(Team) );

  team->first_worker = -1;
  team->workers = NULL;
  team->next_worker = NULL;

  return team;
}

void team__init_arrays( Team *team, int start_abs_mins ) {
  int i;

  team->workers = ALLOC_N( Worker, 900 );
  team->next_worker = ALLOC_N( int, 900 );
  team->first_worker = 0;

  for (i=0;i<900;i++) {
    team->workers[i].id = i;
    team->workers[i].productivity = 1.0;
    team->workers[i].ready_abs_mins = start_abs_mins;
    team->next_worker[i] = i + 1;
  }

  team->next_worker[899] = -1;

  return;
}

void team__sort_first_worker( Team *team ) {
  Worker *updated_worker = team->workers + team->first_worker;
  int elf_id = updated_worker->id;
  int prev_elf_id = -1;
  int ready_abs_mins = updated_worker->ready_abs_mins;

  Worker *next_worker;
  int next_elf_id = team->next_worker[ elf_id ];
  while ( next_elf_id > -1 ) {
    next_worker = team->workers + next_elf_id;
    if ( next_worker->ready_abs_mins > ready_abs_mins ||
        ( next_worker->ready_abs_mins == ready_abs_mins && next_elf_id > elf_id ) ) {
      break;
    }
    prev_elf_id = next_elf_id;
    next_elf_id = team->next_worker[ next_elf_id ];
  }

  // If nothing has changed, nothing to do
  if ( next_elf_id == team->next_worker[ elf_id ] ) {
    return;
  }

  team->first_worker = team->next_worker[ elf_id ];
  team->next_worker[ prev_elf_id ] = elf_id;
  team->next_worker[ elf_id ] = next_elf_id;
  return;
}

void team__destroy( Team *team ) {
  // Free the workers!
  xfree( team->workers );
  xfree( team->next_worker );
  xfree( team );
  return;
}

Worker *team__first_worker( Team *team ) {
  return team->workers + team->first_worker;
}
