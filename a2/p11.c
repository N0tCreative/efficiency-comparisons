/* Jerad Arnold
 * 1008362
 * 05-02-2020
 * Cis 3490 a2
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <math.h>

int bruteForce(int* arr, int size);

//feeds all data to the algorithm one line at a time
int main(int argc, char* argv[]) {
	struct timeb start, end;
	int time = 0;
	FILE* fptr;
	int numInts =1;
	int* arr = malloc(sizeof(int));
	int tmpInt =0;
	int invCount =0;
	char temp[100];
	//if no file is stated itll just read data_1.txt from current directory
	if(argc > 1){
		fptr = fopen(argv[1], "r");
	} else {
		fptr = fopen("data_1.txt", "r");
	}

	if(!fptr) {
		printf("no file found");
		return -1;
	}

	//start timer and begin reading from file
	
	while(fgets(temp, 100, fptr) != NULL) {
		//checks each character to see if its a number
		for(int i=0; i<strlen(temp);i++) {
			if(isdigit(temp[i])) { //if it is then read it in as a float
				sscanf(&(temp[i]), "%d", &(tmpInt));
				arr[numInts-1] = tmpInt;
				while(isdigit(temp[i])) { //ignore the rest of the number and begin searching for the next number 
					i++;
				}
			} else if (temp[i] == '\0' || temp[i] == '\n') {
				break;
			}
			numInts++;
			arr =realloc(arr, sizeof(int) * (numInts));
			if(!arr) {
				printf("no more memory1\n");
				return -1;
			}
		}//end of for length of string
		
	}//end of input from file
	fclose(fptr);
	arr =realloc(arr, sizeof(int) * (--numInts));
	if(!arr) {
		printf("no more memory2\n");
		return -1;
	}
	ftime(&start);
	invCount =bruteForce(arr, numInts);
	ftime(&end);
	//after finished file stop timer and calculate how long it was running then output data
	time = 1000*(end.time -start.time) + (end.millitm -start.millitm);
	printf("~~~~~~~~~~~~~~~~\nglobal inversion count = %d\nrun time = %dms\n", invCount, time);
	free(arr);
	return 1;
}

int bruteForce(int* arr, int size) {
	int count =0;
	//check if theres a number bigger in the remaining array (not including current)
	for(int i=0; i <= size-2; i++) {
		for(int j=i+1; j <= size-1; j++) {
			if(arr[i] > arr[j]){
				count++;
			}
		}
	}
	return count;
}