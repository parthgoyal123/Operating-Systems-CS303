/*  CS303 OPERATING SYSTEMS
			LAB7
	Designing a Virtal Memory Manager
				- Parth Goyal (2017csb1095)
*/

// Include the Required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

// Define some Macros to be used throught the code
#define MASK 0x00FF             // Extracts the 8 LSB bits from virtual address
#define PAGE_SHIFT 8            // Number of bits to shift virtual address to obtain the page number
#define BUFFER_SIZE 10          // Address buffer size (in char) -> Taken input as char but converted to int
#define TLB_SIZE 16             // 16 entries in the TLB
#define NUM_FRAMES 256          // Number of frames
#define FRAME_SIZE 256          // Size of each frame 256-byte (in Physical Memory)
#define NUM_PAGES 256           // Number of pages in the Page Table (in Virtual Memory)
#define PAGE_SIZE 256           // Size of each page 256-byte

// Prototypes of initialize functions
void initializeTLB(int **TLB);
void initializePageTable(int *page_table);
void initializePhysicalMemory(signed char **physical_memory);

// Prototypes of functions to be performed regularly
int checkTLB(int **TLB, int page_number);
int checkPageTable(int *page_table, int page_number);
int addToPageTable(int *page_table, int page_number, int frame_number);

// timestamp
int timestamp = 0;

// Main function
int main(int argc, char *argv[]) {

	// define the file name to be used in this assignment
	// (the names of the file are taken in the form of command line arguments)
	char *address_filename;
	char *backing_store_filename;

	// check if the correct arguments were provided
	if(argc != 3) {
		fprintf(stderr,"Usage: ./vm [backing store] [input file]\n");
		return -1;
	}

	// At this point, we have the names of address and backing_store files
	address_filename = argv[2];
	backing_store_filename = argv[1];

	// Open the Addresses file
	FILE *address_file = fopen(address_filename, "r");

	// check if the address file opens or not
	if(address_file == NULL) {
		fprintf(stderr, "Error in opening the Addresses file\n");
		return -1;
	}

	// Open the Backing Store File ("rb" - read binary)
	FILE *backing_store_file = fopen(backing_store_filename, "rb");

	// check if the backing_store file opens or not
	if(backing_store_file == NULL) {
		fprintf(stderr, "Error in opening the BACKING STORE file\n");
		return -1;
	}

	// index counter
	int i = 0; 

	//* Now let us initialize the TLB, page table and physical memory
	signed char **physical_memory = (signed char**)malloc(NUM_FRAMES*sizeof(signed char*));
	
	for(i = 0; i < NUM_FRAMES; i++) {
		physical_memory[i] = (signed char*)malloc(FRAME_SIZE*sizeof(signed char));
	}
	
	int *page_table = (int*) malloc(NUM_PAGES*sizeof(int));

	int **TLB = (int**)malloc(TLB_SIZE*sizeof(int*));
	
	for(i = 0; i < TLB_SIZE; i++) {
		TLB[i] = (int*)malloc(3*sizeof(int));
	}

	//* Initialize all the tables
	initializePhysicalMemory(physical_memory);
	initializeTLB(TLB);
	initializePageTable(page_table);


	//* Initialize some variables for statistics
	int TLB_hits = 0;
	int total_operations = 0;
	int page_faults = 0;
	int frame_counter = 0;

	// Define the address to be read from the address_file
	char address[BUFFER_SIZE];

	// Read all the addresses and act accordingly
	while(fgets(address, BUFFER_SIZE, address_file) != 0) {

		// increment the total number of operations
		total_operations++;

		// Get the logical address (as int) from (char) address
		int logical_address = atoi(address);

		// Get the page number and offset
		int page_number = (logical_address >> PAGE_SHIFT) & MASK;
		int offset = (logical_address & MASK);

		// Check if the page is in the TLB
		int frame_number = checkTLB(TLB, page_number);

		// if not found in TLB
		if(frame_number == -1) {

			// check the page table for the page_number
			frame_number = checkPageTable(page_table, page_number);

			// if page table doesn't have the page_number entry
			if(frame_number == -1) {

				// create a frame number from a counter and add it to the page table
        		frame_number = addToPageTable(page_table, page_number, frame_counter);

        		// increment the frame counter
				frame_counter = (frame_counter + 1) % NUM_FRAMES;
				
				// increment number of page faults
				page_faults++;
				
				// navigate to the page number in the backing store
				if (fseek(backing_store_file, page_number * PAGE_SIZE, SEEK_SET) != 0) {
					fprintf(stderr, "Error seeking in backing store\n");
					return -1;
				}
		
				//read FRAME_SIZE bytes into physical memory
				if (fread(physical_memory[frame_number], sizeof(signed char), FRAME_SIZE, backing_store_file) == 0) {
					fprintf(stderr, "Error reading from backing store\n");
					return -1;
				}
			} 

			// add this page to TLB and update timestamp
			// update timestamp of the page_number
			bool page_added_to_TLB = false;

			for(i = 0; i<TLB_SIZE; i++) {
				
				// if some location is empty
				if(TLB[i][0] == -1) {
					TLB[i][0] = page_number;
					TLB[i][1] = frame_number;
					TLB[i][2] = timestamp++;
					page_added_to_TLB = true;
					break;
				}
			}

			// if TLB was full, then remove LRU and add this page
			if(page_added_to_TLB == false) {

				int lru = INT_MAX;
				int lru_index;
				for(i = 0; i<TLB_SIZE; i++) {
					
					// if some location is empty
					if(TLB[i][2] < lru) {
						lru_index = i;
						lru = TLB[i][2];
					}
				}
				page_added_to_TLB = true;
				
				TLB[lru_index][0] = page_number;
				TLB[lru_index][1] = frame_number;
				TLB[lru_index][2] = timestamp++;
			}

		}

		// if found in TLB
		else {
			// increment the TLB hits
			TLB_hits++;

			// update timestamp of the page_number
			for(i = 0; i<TLB_SIZE; i++) {
				if(TLB[i][0] == page_number) {
					TLB[i][2] = timestamp++;
				}
			}
		}

		// get the physical address from the frame number
		int physical_address = frame_number << 8;
		physical_address = physical_address | offset;
		
		// get the requested value  
		signed char read_value = physical_memory[frame_number][offset];  
		
		// print data
		printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, read_value);
	}

	//print summary
	printf("\nNumber of Translated Addresses = %d\n", total_operations);
	printf("Page Faults = %d\n", page_faults);
	printf("Page Fault Rate = %0.2f%c\n", (double)page_faults/ (double)total_operations * (double)100, '%');
	printf("TLB Hits = %d\n", TLB_hits);
	printf("TLB Hit Rate = %0.2f%c\n", (double)TLB_hits / (double)total_operations * (double)100, '%');
	printf("----------------------------------------------------------\n");
	fclose(address_file);
	fclose(backing_store_file);

	return 0;
}

// Definition of initializeTLB function()
void initializeTLB(int **TLB) {
	/**
	 * Initializes the TLB with -1 denoting empty TLB.
	 */
	int i = 0;
	for(; i<TLB_SIZE; i++) {

		// -1 denotes empty
		TLB[i][0] = -1;     // page_number at index 0
		TLB[i][1] = -1;     // frame_number at index 1
		TLB[i][2] = -1;		// timestamp
	}
}

// Definition of initializePageTable function()
void initializePageTable(int *page_table) {
	/**
	 * Initializes the page_table with -1 denoting empty page table.
	 */
	int i = 0;
	for(; i<PAGE_SIZE; i++) {

		// -1 denotes empty
		page_table[i] = -1;
	}
}

// Definition of initializePhysicalMemory function()
void initializePhysicalMemory(signed char **physical_memory) {
	/**
	 * Initializes the physical_memory with -1 denoting empty physical_memory.
	 */
	
	int i = 0, j = 0;
	for(; i<NUM_PAGES; i++) {
		for(; j<PAGE_SIZE; j++) {

			// 0 denotes empty space in memory
			physical_memory[i][j] = 0;
		}
	}
}

// Definition of checkTLB function()
int checkTLB(int **TLB, int page_number) {
	/**
	* Checks if a certain page_number is in the TLB or not.
	* Arguments - 
	*     page_number and TLB
	* Returns - 
	*     {
	*         -1 : if not exists
	*         frame_number : if exists, then the frame number
	*     }
	*/
	int i = 0;
  
  	for(; i < TLB_SIZE; i++) {
    	if(TLB[i][0] == page_number) {
			TLB[i][2] = timestamp;
			timestamp = timestamp + 1;
      		return TLB[i][1];
		}
 	}
	
	// if not found in TLB
  	return -1;
}

// Definition of checkPageNumber function()
int checkPageTable(int *page_table, int page_number) {
	/**
	* Checks if a certain page_number is in the page table or not.
	* Arguments - 
	*     page_number and page table
	* Returns - 
	*     {
	*         -1 : if not exists
	*         frame_number : if exists, then the frame number
	*     }
	*/
	return page_table[page_number];
}

// Definition of addToPageTable function()
int addToPageTable(int *page_table, int page_number, int frame_number) {
	/**
	 * Adds frame_number to page_number
	*/
	page_table[page_number] = frame_number;
	return frame_number;
}