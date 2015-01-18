// ext/factory_elf/rub_module_factory_elf.c

#include "ruby_module_factory_elf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

VALUE FactoryElf = Qnil;
VALUE FactoryElf_Worker = Qnil;
VALUE FactoryElf_Clock = Qnil;
VALUE FactoryElf_Schedule = Qnil;
VALUE FactoryElf_ScheduleItem = Qnil;
VALUE FactoryElf_Toy = Qnil;
VALUE FactoryElf_Team = Qnil;
VALUE FactoryElf_Office = Qnil;

void init_module_factory_elf() {
  FactoryElf = rb_define_module( "FactoryElf" );
  FactoryElf_Worker = rb_define_class_under( FactoryElf, "Worker", rb_cObject );
  FactoryElf_Clock = rb_define_module_under( FactoryElf, "Clock" );
  FactoryElf_Schedule = rb_define_class_under( FactoryElf, "Schedule", rb_cObject );
  FactoryElf_ScheduleItem = rb_define_class_under( FactoryElf, "ScheduleItem", rb_cObject );
  FactoryElf_Toy = rb_define_class_under( FactoryElf, "Toy", rb_cObject );
  FactoryElf_Team = rb_define_class_under( FactoryElf, "Team", rb_cObject );
  FactoryElf_Office = rb_define_class_under( FactoryElf, "Office", rb_cObject );

  init_class_worker();
  init_module_clock();
  init_class_schedule();
  init_class_schedule_item();
  init_class_toy();
  init_class_team();
  init_class_office();
}
