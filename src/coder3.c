#include "coder3.h"

#include <string.h>
#include "coder2.h"

/*
 * This function is fully implemented.  You should use this as a reference
 * for implementing the remaining instruction types.
 */
void iplc_sim_process_pipeline_rtype(char *instruction, int dest_reg, int reg1, int reg2_or_constant)
{
    /* This is an example of what you need to do for the rest */
    // assign vals to instruction specific vars();
    iplc_sim_push_pipeline_stage();
    
    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = RTYPE; 
    pipeline[FETCH].instruction_address = instruction_address;
    
    // assign vals to instruction specific vars
    strcpy(pipeline[FETCH].stage.rtype.instruction, instruction);
    pipeline[FETCH].stage.rtype.reg1 = reg1;
    pipeline[FETCH].stage.rtype.reg2_or_constant = reg2_or_constant;
    pipeline[FETCH].stage.rtype.dest_reg = dest_reg;
}

void iplc_sim_process_pipeline_lw(int dest_reg, int base_reg, unsigned int data_address)
{
    iplc_sim_push_pipeline_stage();

    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = LW;
    pipeline[FETCH].instruction_address = instruction_address;

    // assign vals to instruction specific vars
    pipeline[FETCH].stage.lw.data_address = data_address;
    pipeline[FETCH].stage.lw.dest_reg = dest_reg;
    pipeline[FETCH].stage.lw.base_reg = base_reg;
}

void iplc_sim_process_pipeline_sw(int src_reg, int base_reg, unsigned int data_address)
{
    iplc_sim_push_pipeline_stage();

    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = SW;
    pipeline[FETCH].instruction_address = instruction_address;

    // assign vals to instruction specific vars
    pipeline[FETCH].stage.sw.data_address = data_address;
    pipeline[FETCH].stage.sw.src_reg = src_reg;
    pipeline[FETCH].stage.sw.base_reg = base_reg;
}

void iplc_sim_process_pipeline_branch(int reg1, int reg2)
{
    iplc_sim_push_pipeline_stage();

    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = BRANCH;
    pipeline[FETCH].instruction_address = instruction_address;

    // assign vals to instruction specific vars
    pipeline[FETCH].stage.branch.reg1 = reg1;
    pipeline[FETCH].stage.branch.reg2 = reg2;
}

void iplc_sim_process_pipeline_jump(char *instruction)
{
    iplc_sim_push_pipeline_stage();

    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = JUMP;
    pipeline[FETCH].instruction_address = instruction_address;
    
    // assign vals to instruction specific vars
    strcpy(pipeline[FETCH].stage.jump.instruction, instruction);
}

void iplc_sim_process_pipeline_syscall()
{
    iplc_sim_push_pipeline_stage();

    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = SYSCALL;
    pipeline[FETCH].instruction_address = instruction_address;
}

void iplc_sim_process_pipeline_nop()
{
    iplc_sim_push_pipeline_stage();
    
    // assign vals to itype + instruction_address
    pipeline[FETCH].itype = NOP;
    pipeline[FETCH].instruction_address = 0;
}
