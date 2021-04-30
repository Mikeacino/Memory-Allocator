/**
 * Project: Custom Memory Allocator
 * Author: Michael Carracino
 * Date April 2021
 * Description: 
 *      This is a custom version of malloc() and free(). It has been used to understand the structure and logic of memory allocators.
 *      It has also been used to test the logic behind a memory allocator, but has been bogged down in trying to simulate the environment.
 *      This is largely just Sunk Cost Fallacy, as a more accurate simulator might let me refine the logic, and cut half of this code, 
 *      but a more realistic simulator would take more time to build and debug. If this code is unfinished, or missing components, it is
 *      because I switched over to C before completing this program. Most of this program has been complications from the simulation. 
 *      ASSEMBLY would be faster to code this in, by this point.
**/ 
#include <iostream>
#include <stack>
#include <ctime>
#include <list>

using namespace std;

//These are my simulation conditions, in bytes
int maxMemory = 1024;       //Total space of 256 bytes.
int headerSize = 2;         //Header is one word in length.
int heapStartPointer = 0;
int heapEndPointer = 0;   //This is to prevent us from over filling our simulated memory.
//int pointerSize = 4; //Pointers are 4 bytes in length;

class MemoryBlock {
    public:
        int startPointer = -1;
        int length = -1;
        int nextBlockPointer = -1;
        bool is_free = true;
        int id = -1;
};

int counterID = 0;

//This will be our substitute for the memory heap we would deal with in the main problem.
//    A stack of objects is a cheeky way to simulate the real thing.
stack<MemoryBlock> memoryHeap;
list<MemoryBlock> freeList;

//This is my custom memory allocator. It does most of the logic that my C program will be doing.
int myMalloc(int sizeIn){
    MemoryBlock dummyBlock;
    int deletedBlock = 0;
    dummyBlock.length = sizeIn;
    dummyBlock.is_free = false;
    dummyBlock.id = counterID;
    counterID += 1;
    std::list<MemoryBlock>::iterator it;
    if(freeList.size() != 0){           //Check the freeList first!
        for (it = freeList.begin(); it != freeList.end(); it++){
            if (it->is_free && it->length < sizeIn){        //Splitting here, fits the new block inside of an old free block if possible
                deletedBlock = it->id;
                it->is_free = true;
                dummyBlock.is_free = false;
                dummyBlock.startPointer = it->startPointer; //New block starts where old free block was
                return 0;                                   //Memory was placed in a free block successfully, so break.
            }
        }
    } else {}                            //Freelist is empty, do the next thing!
    //Place the memory in a new location
    if(memoryHeap.size() == 0){         //Start memory at first available byte
        dummyBlock.startPointer = 0;
        memoryHeap.push(dummyBlock);
    } else if(memoryHeap.size() == maxMemory){              //Uhhh, this should crash, but let's not worry about that now
        cout << "Out of memory! Please have a nice day!" << endl;
        return -1;
    } else {                            //Start after the last used block
        dummyBlock.nextBlockPointer = memoryHeap.top().startPointer;
        dummyBlock.startPointer = heapEndPointer;
        memoryHeap.push(dummyBlock);
    };
    heapEndPointer += dummyBlock.length + headerSize;   
    return 0;
}
//This is my version of free(). It adds items to the free list, but it has to do a lot of extra work because of the simulation
int myFree(int fakePointer){
    stack<MemoryBlock> dummyHeap;
    MemoryBlock dummyMemoryBlock;
    //To find the element to be freed, we need to loop through the heap.
    for (int i = 0; i < memoryHeap.size(); i++){
        dummyMemoryBlock = memoryHeap.top();
        memoryHeap.pop();
        if(dummyMemoryBlock.id == fakePointer){     //Found the block to be free
            dummyMemoryBlock.is_free = true;
        }
        dummyHeap.push(dummyMemoryBlock);
    }
    memoryHeap = dummyHeap;     //Side Effect of handling weird objects in C++, might be updated if i find a better object
    return 0;
}

//This is for filling the memory for faster debugging, can run over the byte limit
void addRandomMemory(int startNum, int range, int iterations){
    srand(time(0));
    int dummyRand = 0;
    for (int i = 0; i < iterations; i++)
    {
        dummyRand = rand() % range + startNum;
        myMalloc(dummyRand);
    }
}

//Prints the data for a single block of memory
void printMemoryBlock(MemoryBlock dummy){
    if(dummy.is_free == false){
        string dummyStr = "\nNext block is at: ";
        if (dummy.startPointer == 0){
            dummyStr = "\nThis block is the first memory block.";
        }
        cout<< "Block ID: [" << dummy.id << "]" <<
        "\nThis block starts at: " << dummy.startPointer << 
        "\nThis block ends at: " << dummy.startPointer + headerSize + dummy.length <<
        "\nLength: " << dummy.length << 
        dummyStr << dummy.nextBlockPointer << endl;
    }
}

//Tries to print the stack contents in a human readable way
void printMemoryHeap(){
    stack<MemoryBlock> tempStack;
    MemoryBlock dummyBlock;
    int dummySize;
    int count = memoryHeap.size();
    if(memoryHeap.size() >= 0){
        cout << "____________________________________" << 
        "\nStack Fullness: "<< heapEndPointer-heapStartPointer << "/" << maxMemory <<
        "\nHeader Size is: " << headerSize <<
        "\nAllocated memory uses bytes: [" << heapStartPointer << " - " << heapEndPointer << 
        "]\n____________________________________" << endl;
        for (int i = 0; i < count; i++){
            dummyBlock = memoryHeap.top();
            cout << "____________" << endl;
            printMemoryBlock(dummyBlock);
            memoryHeap.pop();
        }
    } else {
        cout << "Memory is completely free." << endl;
    }
};

//Not Implemented yet. Should count the gaps in between memory blocks, to give an idea of the level of fragmentation
void fragmentationCalc(){
    for (int i = 0; i < memoryHeap.size(); i++)
    {
        memoryHeap.top();
    }
}

int main() {
    addRandomMemory(10, 100, 13);
    MemoryBlock dummy2;
    //myFree(2);
    printMemoryHeap();
    return 0;
};
