// ext/factory_elf/shared_vars.h

#ifndef SHARED_VARS_H
#define SHARED_VARS_H

#define NUM2FLT(x) ((float)NUM2DBL(x))
#define FLT2NUM(x) (rb_float_new((double)x))

// Force inclusion of hash declarations (only Ruby MRI includes by default)
#ifdef HAVE_RUBY_ST_H
#include "ruby/st.h"
#else
#include "st.h"
#endif

extern VALUE FactoryElf;
extern VALUE FactoryElf_Clock;
extern VALUE FactoryElf_Schedule;
extern VALUE FactoryElf_ScheduleItem;
extern VALUE FactoryElf_Toy;
extern VALUE FactoryElf_Worker;
extern VALUE FactoryElf_Team;
extern VALUE FactoryElf_Office;

#endif
