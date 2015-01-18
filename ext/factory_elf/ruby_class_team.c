// ext/factory_elf/ruby_class_team.c

#include "ruby_class_team.h"

//////////////////////////////////////////////////////////////////////////////////////////////////

inline VALUE team_as_ruby_class( Team *team , VALUE klass ) {
  return Data_Wrap_Struct( klass, NULL, team__destroy, team );
}

VALUE team_alloc(VALUE klass) {
  return team_as_ruby_class( team__create(), klass );
}

inline Team *get_team_struct( VALUE obj ) {
  Team *team;
  Data_Get_Struct( obj, Team, team );
  return team;
}

void assert_value_wraps_team( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)team__destroy) {
    rb_raise( rb_eTypeError, "Expected a FactoryElf_Team object, but got something else" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VALUE team_object_initialize( VALUE self, VALUE rv_start_abs_mins ) {
  Team *team = get_team_struct( self );
  team__init_arrays( team, NUM2INT(rv_start_abs_mins) );
  return self;
}

VALUE team_object_sort_first_worker( VALUE self ) {
  Team *team = get_team_struct( self );
  team__sort_first_worker( team );
  return self;
}

VALUE team_object_first_worker( VALUE self ) {
  Team *team = get_team_struct( self );
  Worker *team_member = team__first_worker( team );

  // We clone, because otherwise Ruby would GC team members
  Worker *worker = worker__create();
  worker->id = team_member->id;
  worker->productivity = team_member->productivity;
  worker->ready_abs_mins = team_member->ready_abs_mins;

  volatile VALUE rv_worker = Data_Wrap_Struct( FactoryElf_Worker, NULL, worker__destroy, worker );

  return rv_worker;
}

VALUE team_object_all_workers( VALUE self ) {
  Team *team = get_team_struct( self );
  Worker *team_member = team__first_worker( team );
  volatile VALUE rv_workers = rb_ary_new2( 900 );
  int i, elf_id;

  for(i=0;i<900;i++) {
    // We clone, because otherwise Ruby would GC team members
    Worker *worker = worker__create();
    worker->id = team_member->id;
    worker->productivity = team_member->productivity;
    worker->ready_abs_mins = team_member->ready_abs_mins;

    rb_ary_store(
      rv_workers,
      i,
      Data_Wrap_Struct( FactoryElf_Worker, NULL, worker__destroy, worker )
    );

    elf_id = team->next_worker[ team_member->id ];
    if ( elf_id > 899 || ( elf_id < 0 && i < 899 ) ) {
      rb_raise( rb_eArgError, "Something has gone wrong with team data structure, not enough elves, pos %d, elf_id %d", i, elf_id );
    }

    // Next team member
    team_member = team->workers + elf_id;
  }

  return rv_workers;
}

VALUE team_object_update_first_worker( VALUE self, VALUE rv_worker ) {
  Team *team = get_team_struct( self );
  Worker *team_member = team__first_worker( team );
  Worker *worker;
  Data_Get_Struct( rv_worker, Worker, worker );

  if ( team_member->id != worker->id ) {
    rb_raise( rb_eArgError, "Elf id %d not same as first elf %d", worker->id, team_member->id );
  }

  team_member->productivity = worker->productivity;
  team_member->ready_abs_mins = worker->ready_abs_mins;

  return self;
}

void init_class_team() {
  rb_define_alloc_func( FactoryElf_Team, team_alloc );
  rb_define_method( FactoryElf_Team, "initialize", team_object_initialize, 1 );
  rb_define_method( FactoryElf_Team, "all_workers", team_object_all_workers, 0 );

  rb_define_method( FactoryElf_Team, "first_worker", team_object_first_worker, 0 );
  rb_define_method( FactoryElf_Team, "update_first_worker", team_object_update_first_worker, 1 );
  rb_define_method( FactoryElf_Team, "sort_first_worker", team_object_sort_first_worker, 0 );
  return;
}
