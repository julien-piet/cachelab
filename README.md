# Cachelab submission

This is my submission to the INF559 course's cachelab at polytechnique. The goal was to implement our own cache, and write a cache miss minimizing implementation of a specific matrix multiplication problem.
Although this is from 2017, you can find the new assignment on this page : 
https://www.enseignement.polytechnique.fr/informatique/INF559/TD/cachelab/INF559-cachelab-1.php

The relevant files to the submission are csim.c and trans.c. This submission got the maximal score.

Below are the instructions for running the grading system.

This is the handout directory for the CS:APP Cache Lab. 

************************
Running the autograders:
************************

Before running the autograders, compile your code:
    linux> make

Check the correctness of your simulator:
    linux> ./test-csim

Check the correctness and performance of your transpose functions:
    linux> ./test-trans -M 32 -N 32
    linux> ./test-trans -M 64 -N 64
    linux> ./test-trans -M 61 -N 67

Check everything at once (this is the program that your instructor runs):
    linux> ./driver.py    

******
Files:
******

You will modifying and handing in these two files
csim.c       Your cache simulator
trans.c      Your transpose function

Tools for evaluating your simulator and transpose function
Makefile     Builds the simulator and tools
README       This file
driver.py*   The driver program, runs test-csim and test-trans
cachelab.c   Required helper functions
cachelab.h   Required header file
csim-ref*    The executable reference cache simulator
test-csim*   Tests your cache simulator
test-trans.c Tests your transpose function
tracegen.c   Helper program used by test-trans
traces/      Trace files used by test-csim.c
