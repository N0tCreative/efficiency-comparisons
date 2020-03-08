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

int divAndConq(int* arr, int size);

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
	invCount =divAndConq(arr, numInts);
	ftime(&end);
	//after finished file stop timer and calculate how long it was running then output data
	time = 1000*(end.time -start.time) + (end.millitm -start.millitm);
	printf("~~~~~~~~~~~~~~~~\nglobal inversion count = %d\nrun time = %dms\n", invCount, time);
	free(arr);
	return 1;
}

int divAndConq(int* arr, int size) {
	int count =0;
	int* arrb;
	int* arrc;
	//if the array size is 1 just return count =0 because no swapping needs to be done
	if(size >1) {
		//make 2 sub arrays and populate them with first and second half of original array
		arrb = malloc (sizeof(int) * (size/2));
		arrc = malloc (sizeof(int) * ceil(size/2.0));
		for(int i=0; i < (size/2); i++) {
			arrb[i] = arr[i];
		}
		for(int i=(size/2); i < size; i++) {
			arrc[i-(size/2)] = arr[i];
		}

		//sort the first subarray and count the amount of numbers out of place
		count +=divAndConq(arrb, (size/2));
		//sort the second subarray and count the amount of numbers out of place
		count +=divAndConq(arrc, (int)ceil(size/2.0));

		//set values to use for comparing each subarray
		int i=0, j=0, k=0;
		//make sure that neither subarray goes out of bounds and replace all elements in original array with a sorted version of the sub array ones
		while(i < (size/2) && j < (int)ceil(size/2.0)) {
			//if the first subarray has an element bigger than the second add the one from the second to the original array
			if(arrb[i] > arrc[j]) {
				arr[k++] = arrc[j++];
				//add the number of elements on the first array that are bigger than the current element of the second array (the # of elements in that array remaing) to the count
				count += (size/2)-i;
			}else { //otherwise just add the element of the first array
				arr[k++] = arrb[i++];
			}
			
		}
		//if all elements in the first array are copied then copy the rest of the second array to the original array
		if(i>=(size/2)) {
			while(j < (int)ceil(size/2.0)) {
				arr[k++] = arrc[j++];
			}
		} else { //if all elements in the second array are copied then copy the rest of the first array to the original array
			while(i < size/2) {
				arr[k++] = arrb[i++];
			}
		}
		//free all malloc'd memory
		free(arrb);
		free(arrc);
	}
	//return the current count
	return count;
}