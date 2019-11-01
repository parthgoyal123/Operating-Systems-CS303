/*
    Name - Parth Goyal
    Entry Number - 2017csb1095
    Question No. 5 Lab6
    Operating Systems CS303
    Date of submission - 23/10/19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Main function
int main(int argc, char* argv[]) {

    // check if the number of arguments are valid or not
	if(argc <= 1) {
		printf("No addresses entered. Enter atleast 1 address.\n");
		return 0;
	}

    printf("\n");

    // traverse for all the addresses input to get page number and offset
	int i;
	for(i = 1; i < argc; i++){
	
        // address should be positive and unsigned
		if(argv[i][0] == '-') {
			printf("%d. InputError: Address %s entered is invalid!\n\n", i, argv[i]);
			continue;
		}

        char *end;

        // convert the input string to number
		unsigned long long number = strtoull(argv[i], &end, 10);

        // check if the number is greater than 32 bits
		if((long long)number >= (long long)pow(2, 32)) {
			printf("Address entered exceeds 32 bits!\n\n");
			continue;
		}

		printf("%d. The address %llu contains:\n", i, number);

		/* We have 4KB page size which will need 12 bits to be stored. Thus offset bits = 12 and remaining 
		   20 bits will be storing page number. */
	
	    //right shifting the address by 12 bits and extracting 20 bits of address as page number
		printf("page number = %llu\n", number >> 12);

		//extracting 12 bits of address as offset
		printf("offset = %llu\n\n", number & 4095);
	}

	return 0;
}