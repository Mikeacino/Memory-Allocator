# Memory-Allocator
A custom memory allocator as a final project
Currently allocates memory based on the memory_start variable. 
This isn't working right in VSC on Windows. 
Apparentyl sbrk() is the solution, but it's hard to test if i have to switch to linux every time i use it.
The C++ file is the old clunky simulated version.
The C version is the actual allocator.
Plans for testing the speed against other allocators fell through. The other repositories I tried to use were either not working or too specialized. I tried following the steps to add a enw alloctor in the files, but there must have been other changes to my code that were needed.
