/* --------------- LAB3 Operating SYstems CS303 ----------------- */
					Submitted by - Parth Goyal (2017csb1095)

Question 1 :

Simply getpid() function is used and printed in the output file
Command : 
1. gcc -o Q1 Q1.c && ./Q1 > output_Q1.txt

Question 2 :

The parent process sleeps for 15 seconds and the child process becomes Zombie meanwhile
Command : 
1. ps -l 
2. gcc -o Q2 Q2.c && ./Q2 &
3. ps -l
4. kill -9 "Zombie process PID"

Question 3 :

The parent process waits until the child process is complete to run the 
algorithm successfully.
1. gcc -o Q3 Q3.c
2. ./Q3 "Your Number" ex. : ./Q3 35

Question 4 :

Shared Memory block is printed
1. gcc -o Q4 Q4.c -lrt
2. ./Q4 "Your Number" ex. : ./Q4 35


