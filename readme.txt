Readme

Editor used: vim
Compile using gcc
output: ./a.out

The aim of the project is to encode a given wedding arrangement constraints into CNF’s and hence sentences.
Later verify the CNF generated sentences to be satisfiable or not, via PL-resolution and Walk-SAT algorithms

The following are the files attached

CNF.h
PL_Resolution.c
walk_SAT.c
exp3_walk_SAT.c


PL_Resolution.c
This is a time consuming program and took me a really long time to generate the 500 random sentences (50 for each probability).
As I am using C and doing manual memory allocation and de-allocation this is adding to the time. 
The program is interactive and in the beginning would print as shown below

If you want to generate a single sentence press 1 else press 2 to generate random sentences?
1
Enter the number of people you wish to have in the wedding
10
Enter the number of tables you wish to have in the wedding
2
Enter the percentage probability (please enter only numbers out of 100) of two people being friends
2
Enter the percentage probability (please enter only numbers out of 100) of two people being enemies
4
Encoding ...

If the user presses 2 then it leads him to task 4, of PL-resolution where we need to generate 50 random sentences for <e> ranging from 2 to 20% increments of 2. 

Observation: probability 8 onwards the system becomes slow and uses great extent of CPU. To overcome the wait, I have asked the user to input the probability and the number of sentences (give 10 or max 20 as the input) at a time.
 As the numbers are generated in random fashion this does not affect the output much and helps to lower CPU utilization as compared to firing 500 at once.

The output will be redirected to a file (as it should not spam the screen), under the same directory as the program.
PL-ResolutionOutput.txt


Walk-SAT part one

File: walk_SAT.c

This is a fast program. This is again interactive 
The user is asked if he wants to generate random sentences 
If NO, then he can create a single sentence and give the required max_flips and check if its satisfiable, else he could say yes and 
generate either TASK 4 or TASK 5.
For random generations that is part of TASK 4, TASK 5 the output will be redirected to the file
WalkSat_Output.txt 

Walk-SAT part two


The experiment 3 Task 6 for walk sat has a separate program
File: exp3_walk_SAT.c

The output of this would be present in 
WalkSAT_Output_experiment3.txt









