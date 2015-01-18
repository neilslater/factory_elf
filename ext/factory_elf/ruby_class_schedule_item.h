// ext/factory_elf/ruby_class_schedule_item.h

#ifndef RUBY_CLASS_SCHEDULE_ITEM_H
#define RUBY_CLASS_SCHEDULE_ITEM_H

#include <ruby.h>
#include "narray.h"
#include "shared_vars.h"
#include "struct_schedule.h"

void init_class_schedule_item();
void assert_value_wraps_schedule_item( VALUE obj );

#endif
