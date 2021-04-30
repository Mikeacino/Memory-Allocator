/**
 * Project:     Custom Memory Allocator 
 * Author:      Michael Carracino
 * Class:       Operating System Design
 * Description: This is my custom memory allocator. It includes a malloc() and free() variant. Some of the
 *              stuff here like typedef, struct, and union are used with heavy internet support. Mostly 
 *              because i'm not very familiar with it. This is being written in an IDE in windows, so usage
 *              of common functions like sbrk(x) is purely hypothetical until tested elsewhere.
*/
#include <stdio.h>
#include <heapapi.h>
#include <time.h>

typedef char alignment_value[20];   //Use the 'union' to force the size of the header to grow to a min size
void* memory_start;

/** sets up format for header
 * This is our header that allows us to traverse between memory block quickly.
 *      We need to ensure the headers are constant size, so we use a union.
*/
union union_memory_header {
    struct struct_mem_header
        {
            union union_memory_header *next_memory;
            int bytes_included;
            int mem_is_free;
        }HEADY;
    alignment_value dummy;
};
typedef union union_memory_header mem_header;     //This makes the mem_header a data type, i think.
mem_header *head_ptr, *tail_ptr;

/** get_free_block() variant? I know malloc() does this, but i'm not sure if this is integral or seperate
 * Check the free list for an empty block.
 *      Returns null if no viable block was found
 *      Returns a pointer to the available block if one was found
 */
mem_header* check_free_list(int needed_size){
    mem_header* current_block = head_ptr;       //Start at the newest block.
    printf("The next memory is at: %d\n", current_block);
    if(!current_block){
        printf("Definitely zero");
    } else {
        printf("Not zero");
    }
    while(!current_block->HEADY.next_memory){                                    //Go until you hit the last header, which SHOULD be null
        if (current_block->HEADY.bytes_included < needed_size){
            if(current_block->HEADY.mem_is_free){
                printf("Found something");
                return current_block;
            }
        }
        current_block = current_block->HEADY.next_memory;               //Move to the next block
    }
    printf("Found nothing");
    return NULL;            //This would mean no viable memory was found.
}

/** Debugging function
 * prints out the pointers for the Head and Tail. Helps make sure they move correctly
**/
void print_head_tail(){
    printf("---------Begin Head/Tail---------\n");
    printf("The head pointer is at: %d\n", head_ptr);
    printf("The tail pointer is at: %d\n", tail_ptr);
    printf("----------End Head/Tail----------\n");
}

/** Debugging function
 * Prints out the details of a header.
**/
void print_header_details(mem_header* header_ptr){
    printf("--------------Start Header Details--------------\n");
    printf("Header pointer is at: %d\n", header_ptr);
    mem_header dummy_header = *header_ptr;
    printf("Header total size is: %d\n", sizeof(dummy_header));
    printf("Blocks allocated: %d\n", dummy_header.HEADY.bytes_included);
    if(!dummy_header.HEADY.mem_is_free){
        printf("This block is: FREE\n");
    } else {
        printf("This block is: NOT FREE\n");
    }
    if(!dummy_header.HEADY.next_memory){
        printf("This block is the end of the list\n");
    } else {
        printf("The next block is at: %d\n",dummy_header.HEADY.next_memory);
    }
    printf("---------------End Header Details---------------\n");

}

/** malloc() variant
 * This is the bulk of the code, rewritten from a C++ program that got bogged down in the simulation details
 *      It returns a pointer to the new block of memory.
*/
mem_header* mike_malloc(size_t blocksRequested){
    if (!blocksRequested){       //If blocks = 0
        printf("Oops, no memory requested, nothing happens.");
        return NULL;
    } else {
        mem_header new_header;          //This is our new header
        mem_header *new_header_ptr;     //This is the location of our new header
        new_header_ptr = check_free_list(blocksRequested); //Store the result, NULL means do the rest
        if(new_header_ptr){             //The free list returned Not Null
            return new_header_ptr;      //Found a viable free block, back out
        }
        new_header_ptr = &new_header;   //Set the pointer to this new header

        if(head_ptr){                   //If this is not null, move the header to this block.
            new_header_ptr->HEADY.next_memory = head_ptr;     //Store the start of the old header
            head_ptr = new_header_ptr;  //Move the Head to this block
        } else {                        //If this is null, this is the first header ever.
            new_header_ptr->HEADY.next_memory = NULL;
            head_ptr = new_header_ptr;  //Head ptr needs to start somewhere
            tail_ptr = new_header_ptr;  //Tail pointer never moves from the first block.
        }

        //Allocate this much
        size_t total_memory_size = blocksRequested + sizeof(&new_header_ptr);
        head_ptr = sbreak(total_memory_size);   //Move the head to the latest block in memory.

        new_header_ptr->HEADY.bytes_included = blocksRequested;
        new_header_ptr->HEADY.mem_is_free = 1;  //Block is Not Free
        return new_header_ptr;                  //Return the block of memory after the one given by sbrk()
    }
}

/** free() variant
 * This is my free, it frees the memory at a specified block. This requires traversing the free list.
 *      Once it finds the block, it just needs to flip a bit.
**/
void mike_free(void* victim){
    mem_header *current_header = victim;        //cram the pointer into a header pointer
    mem_header current_block = *current_header; //Get the header at that location
    current_block.HEADY.mem_is_free = 0;        //Set the block to free 
}

int main() {
    srand(time(0));                                             //Set a random num
    int rando = rand() % 70 + 30;                               //Set range to [30, 100]
    printf("\n\nI asked for this many blocks: %d\n", rando);    
    mem_header *first_mem = mike_malloc(rando);                 //Just grab the header as if we want it
    return 0;
}
