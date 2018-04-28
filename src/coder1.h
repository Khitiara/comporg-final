// vim: ft=c

#ifndef CODER1_H
#define CODER1_H

#include "base.h"

int iplc_sim_trap_address(unsigned int address);
void iplc_sim_LRU_replace_on_miss(int index, int tag);
void iplc_sim_LRU_update_on_hit(int index, int assoc);

#endif
