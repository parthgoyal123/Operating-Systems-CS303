/* ----------- LAB4 Operating Systems CS303 ------------*/
					Submitted by : Parth Goyal
							(2017csb1095)

Question 1:

During the parent process, close read and write and close the write end of
pipe.
During the child process, close the unused write end and read from the pipe
and close the read end of the pipe.

gcc -o Unix_Pipe_Program Q1.c && ./Unix_Pipe_Program
