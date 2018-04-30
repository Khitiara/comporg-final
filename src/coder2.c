#include "coder2.h"
#include "coder1.h" //for iplc_sim_trap_address
#include <stdio.h>  //for printf
#include <string.h> //for bzero

void iplc_sim_push_pipeline_stage()
{
    //at this point we've just fetched the current instruction.
    //that is, each stage of the pipeline as it is now has been
    //completed. We need to calculate how many cycles this took
    //and then shift the instructions in the pipeline down one.
    //we also need to "insert stalls" (i.e. increment the cycle
    //count) if necessary so that the fetched instruction can run.
    
    /* 1. Count WRITEBACK stage is "retired" -- This I'm giving you */
    if (pipeline[WRITEBACK].instruction_address) {
        instruction_count++;
        if (debug)
            printf("DEBUG: Retired Instruction at 0x%x, Type %d, at Time %u \n",
                   pipeline[WRITEBACK].instruction_address, pipeline[WRITEBACK].itype, pipeline_cycles);
    }
    
    /* 2. Check for BRANCH and correct/incorrect Branch Prediction */
    if(pipeline[DECODE].itype == BRANCH) {
        unsigned ourAddr = pipeline[DECODE].instruction_address;
        unsigned nextAddr = pipeline[FETCH].instruction_address;
        //if the fetched instruction is not the next one in sequence
        //the branch was taken
        int branch_taken = (nextAddr != (ourAddr + 4));
        //if we already stalled due to instruction fetch, it doesn't matter
        if(nextAddr != 0) {
            if(branch_taken == branch_predict_taken) {
                //predictor is correct
                printf("Branch predict correct (address %x -> %x)\n", ourAddr, nextAddr);
                correct_branch_predictions++;
            } else {
                //predictor is incorrect
                //refetching takes an extra cycle
                printf("Branch predict incorrect (address %x -> %x): predict %d, actual %d\n",
                    ourAddr, nextAddr, branch_predict_taken, branch_taken);
                pipeline_cycles++;
            }
        }
        branch_count++;
    }
    
    /* 3. Check for LW delays due to use in ALU stage and if data hit/miss
     *    add delay cycles if needed.
     */
    if(pipeline[MEM].itype == LW) {
        //load word is of the form
        //lw $rd off($rs)
        //we need to check if $rd is being used in the ALU stage, and
        //if so, insert a stall
        int inserted_nop = 0;
        lw_t inst = pipeline[MEM].stage.lw;
        int rd = inst.dest_reg;
        //instruction types lw, sw, rtype use the ALU
        //so we need to check all of them
        switch(pipeline[ALU].itype) {
            case RTYPE: {
                //must check both registers
                rtype_t aluInst = pipeline[ALU].stage.rtype;
                if(aluInst.reg1 == rd || aluInst.reg2_or_constant == rd)
                    inserted_nop = 1;
            }
            break;
            case LW: {
                //check base reg
                if(pipeline[ALU].stage.lw.base_reg == rd)
                    inserted_nop = 1;
            }
            break;
            case SW: {
                //check base reg
                if(pipeline[ALU].stage.sw.base_reg == rd)
                    inserted_nop = 1;
            }
            break;
            default:
                ;
        }
        if(inserted_nop) {
            printf("Load-use stall for use in %d\n", pipeline[ALU].itype);
            pipeline_cycles++;
        }
        //now we need to check the data address we fetched and see if
        //it was in the cache. If not, we must stall.
        if(!iplc_sim_trap_address(inst.data_address)) {
            //stall while data is fetched.
            //Debatable if delaying for 9 cycles is correct,
            //but that's what is in the sample output.
            printf("DATA MISS: Address %x\n", inst.data_address);
            pipeline_cycles += CACHE_MISS_DELAY - 1;
        }
    }
    
    /* 4. Check for SW mem acess and data miss .. add delay cycles if needed */
    if(pipeline[MEM].itype == SW) {
        //sw $rs off($rd)
        //the only thing that can go wrong is a data miss at off($rd)
        unsigned dataAddr = pipeline[MEM].stage.sw.data_address;
        if(!iplc_sim_trap_address(dataAddr)) {
            //stall while data is fetched
            printf("DATA MISS: Address %x\n", dataAddr);
            pipeline_cycles += CACHE_MISS_DELAY - 1;
        }
    }
    
    /* 5. Increment pipe_cycles 1 cycle for normal processing */
    pipeline_cycles++;
    /* 6. push stages thru MEM->WB, ALU->MEM, DECODE->ALU, FETCH->DECODE */
    for(int i = MAX_STAGES - 1; i > 0; i--)
        pipeline[i] = pipeline[i - 1];
    // 7. This is a give'me -- Reset the FETCH stage to NOP via bezero */
    bzero(&pipeline[FETCH], sizeof(pipeline_t));
}
