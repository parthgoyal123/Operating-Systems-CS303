/* ----------- LAB4 Operating Systems CS303 ------------*/
								Submitted by : Parth Goyal
											(2017csb1095)

*Question 1*

During the parent process, close read end and then write and close the write end of
pipe.
During the child process, close the unused write end and read from the pipe
and close the read end of the pipe.

COMMAND --> gcc -o Unix_Pipe_Program Q1.c && ./Unix_Pipe_Program

*Question 2*

During the Parent Process, close read end and write to Pipe1 and close the write end of 
Pipe1 and wait for Child Process.

During the Child process, close the unused write end and read from the pipe1 and close the 
read end of the pipe1. Now Reverse the string read from pipe1 and write to pipe2.

After the child process completes its execution, read from pipe2, that child process wrote.

COMMAND --> gcc -o Unix_2_pipes Q2.c && ./Unix_2_pipes

*Question 3*

Command --> gcc -o Q3 Q3.c && ./Q3

Assumptions/Clarifications:

1. !3 denotes 3rd most recent and not 3rd least recent.

2. !! commands that translates to some other command is added in history.

3. !! & does not work concurrently.

4. All commands whether valid or invalid are added to history table.

5. !! or !3 does not add up in the history if there are no previous commands.