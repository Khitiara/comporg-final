#include "coder1.h"

/*
 * iplc_sim_trap_address() determined this is not in our cache.  Put it there
 * and make sure that is now our Most Recently Used (MRU) entry.
 */
void iplc_sim_LRU_replace_on_miss(int index, int tag)
{
    // replace least recent cache spot
    int replace_index = cache[index].replace[0];

    cache[index].tag[replace_index] = tag;
    cache[index].valid_bit[replace_index] = 1;
    cache[index].replace[cache_assoc - 1] = replace_index;
    
    // translate cache array at index
    for(int i = 0; i < cache_assoc - 1; i++)
        cache[index].replace[i] = cache[index].replace[i + 1];
}

/*
 * iplc_sim_trap_address() determined the entry is in our cache.  Update its
 * information in the cache.
 */
void iplc_sim_LRU_update_on_hit(int index, int assoc_entry)
{
	// search for update index
    int update = 0;
    while(cache[index].replace[update] != assoc_entry)
        update++;

    // translate lru over
    for(int i = update + 1; i < cache_assoc; i++)
        cache[index].replace[i - 1] = cache[index].replace[i];
    
    cache[index].replace[cache_assoc - 1] = assoc_entry;
}

/*
 * Check if the address is in our cache.  Update our counter statistics 
 * for cache_access, cache_hit, etc.  If our configuration supports
 * associativity we may need to check through multiple entries for our
 * desired index.  In that case we will also need to call the LRU functions.
 */
int iplc_sim_trap_address(unsigned int address)
{
    int i = 0
    int index = 0;
    int tag = 0;
    int hit = 0;
    
    unsigned int filter;

    // find index in address
    filter = (1 << (cache_blockoffsetbits + cache_index))--;
    index = address & filter;
    index = index >> cache_blockoffsetbits;

    // find tag in address
    filter = (1 << (cache_blockoffsetbits + cache_index + cache_assoc + 16))--;
    tag = address & filter;
    tag = tag >> (cache_blockoffsetbits + cache_index);

    // check if tag is in cache
    for(i = 0; i < cache_assoc; i++){
        if ((cache[index].valid_bit[i] == 1) && (cache[index].tag[i] == tag)) {
            hit = 1;
            break;
        }
    }

    // access cache
    printf("Address %x: Tag= %x, Index= %x\n", address,tag,index);
    cache_access++;

    // hit or miss functions
    if (hit == 1){
        cache_hit++;
        iplc_sim_LRU_update_on_hit(index, i);
    } else {
        cache_miss++;
        iplc_sim_LRU_replace_on_miss(index, tag);
    }

    return hit;
}
