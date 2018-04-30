// vim: ft=c

#ifndef BASE_H
#define BASE_H

#include <stdlib.h>
#include <stdio.h>

#define MAX_CACHE_SIZE 10240
#define CACHE_MISS_DELAY 10 // 10 cycle cache miss penalty
#define MAX_STAGES 5

void iplc_sim_init(int index, int blocksize, int assoc);
void iplc_sim_parse_instruction(char *buffer);
void iplc_sim_dump_pipeline();
unsigned int iplc_sim_parse_reg(char *reg_str);

// Outout performance results
void iplc_sim_finalize();

typedef struct cache_line
{
    int *valid_bit;
    int *tag;
    int assoc;
    int *replace;
} cache_line_t;

cache_line_t *cache;
int cache_index;
int cache_blocksize;
int cache_blockoffsetbits;
int cache_assoc;
long cache_miss;
long cache_access;
long cache_hit;

char instruction[16];
char reg1[16];
char reg2[16];
char offsetwithreg[16];
unsigned int data_address;
unsigned int instruction_address;
unsigned int pipeline_cycles;   // how many cycles did you pipeline consume
unsigned int instruction_count; // home many real instructions ran thru the pipeline
unsigned int branch_predict_taken;
unsigned int branch_count;
unsigned int correct_branch_predictions;

unsigned int debug;
unsigned int dump_pipeline;

enum instruction_type {NOP, RTYPE, LW, SW, BRANCH, JUMP, JAL, SYSCALL};

typedef struct rtype
{
    char instruction[16];
    int reg1;
    int reg2_or_constant;
    int dest_reg;
    
} rtype_t;

typedef struct load_word
{
    unsigned int data_address;
    int dest_reg;
    int base_reg;
    
} lw_t;

typedef struct store_word
{
    unsigned int data_address;
    int src_reg;
    int base_reg;
} sw_t;

typedef struct branch
{
    int reg1;
    int reg2;
    
} branch_t;


typedef struct jump
{
    char instruction[16];
    
} jump_t;

typedef struct pipeline
{
    enum instruction_type itype;
    unsigned int instruction_address;
    union
    {
        rtype_t   rtype;
        lw_t      lw;
        sw_t      sw;
        branch_t  branch;
        jump_t    jump;
    }
    stage;
    
} pipeline_t;

enum pipeline_stages {FETCH, DECODE, ALU, MEM, WRITEBACK};

pipeline_t pipeline[MAX_STAGES];

#endif
