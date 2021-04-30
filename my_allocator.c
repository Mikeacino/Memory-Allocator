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

typedef char alignment_value[20];   //Use the 'union' to force the size of the header to grow.
void* memory_start;




/**
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




/**
 * Check the free list for an empty block.
 *      Just checks the headers and keep going until the list is done
 *      Returns a pointer, wither null for no luck, or the pointer if valid memory was found.
 *      I really like a lot of the logic in this method.
 *      Returns NULL if no block is found
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




/**
 * This is the bulk of the code, rewritten from a C++ program that got bogged down in the simulation details
 *      It returns a pointer to the new block of memory.
*/
mem_header* mike_malloc(size_t blocksRequested){
    if (!blocksRequested){       //If blocks = 0
        return NULL;
    }

    mem_header* prev_header;     //Store the location of the soon to be previous block
    //Check the Free List! Running into weird errors!
    //if(!check_free_list(blocksRequested)){
        //splitMemory
    //}

    mem_header new_header;          //This is our new header
    mem_header *new_header_ptr;     //This is the location of our new header
    new_header_ptr = &new_header;   //Set the pointer to this new header
    
    if(!head_ptr){             //If this is the first header, set the head and tail.
        prev_header = NULL;
        head_ptr = new_header_ptr;
        tail_ptr = memory_start;
    } else {
        prev_header = head_ptr;     //Store the start of the old header
    }


    //Allocate this much
    printf("Header size is: %d\n", sizeof(new_header));
    size_t total_memory_size = blocksRequested + sizeof(new_header);
    //head = sbreak(total_memory_size);         //Move the head to the location of this new block.
    printf("\nThis is how much to allocate! %d\n", total_memory_size);

    new_header_ptr->HEADY.bytes_included = blocksRequested;
    new_header_ptr->HEADY.mem_is_free = 0;
    int new_size = new_header_ptr->HEADY.bytes_included;
    printf("The size of this header is %d\n", new_size);

    new_header.HEADY.next_memory = prev_header;

    return new_header_ptr;              //Return the block of memory after the one given by sbrk()
}

void mike_free(void* victim){
    
}


int main() {
    srand(time(0));
    int rando = rand() % 70 + 30;
    printf("\n\nI asked for this many blocks: %d\n", rando);
    mem_header* first_mem = mike_malloc(rando);
    rando = rand() % 70 + 30;
    printf("\n\nI asked for this many blocks: %d\n", rando);
    mem_header* second_mem = mike_malloc(rando);
    //mem_header actual_mem = my_mem;
    printf("MY first header is at %d\n", first_mem);
    printf("MY second header is at %d\n", second_mem);
    printf("I recieved this many blocks %d\n", &first_mem->HEADY.bytes_included);
    return 0;
}
