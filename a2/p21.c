/* Jerad Arnold
 * 1008362
 * 08-02-2020
 * Cis 3490 a2
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <math.h>

double** bruteForce(double** arr, int size, int* hullSize);
void divAndConq(double** arr, int size);

//feeds all data to the algorithm
int main(int argc, char* argv[]) {
	struct timeb start, end;
	int time = 0;
	FILE* fptr;
	int count =1;
	double** arr = malloc(sizeof(double*));
	double** hull = NULL;
	double tmpDub =0.0;
	int numHull=0;
	char temp[40];

	//if no file is stated itll just read data_1.txt from current directory
	if(argc > 1){
		fptr = fopen(argv[1], "r");
	} else {
		fptr = fopen("data_2.txt", "r");
	}

	if(!fptr) {
		printf("no file found");
		return -1;
	}

	//begin reading from file
	while(fgets(temp, 40, fptr) != NULL) {
		int i=0;
		arr[count-1]= malloc(2*sizeof(double));
		if(!arr[count-1]) {
			printf("no more memory 0\n");
			return -1;
		}
		//checks each character to see if its a number
		int j=0;
		for(i=0; i<strlen(temp);i++) {
			if(isdigit(temp[i])) { //if it is then read it in as a float
				sscanf(&(temp[i]), "%lf", &(tmpDub));
				arr[count-1][j++] = tmpDub;
				while(isdigit(temp[i]) || temp[i] == '.') { //ignore the rest of the number and begin searching for the next number 
					i++;
				}
				i--;
			} else if (temp[i] == '\0' || temp[i] == '\n') {
				break;
			}
		}
		count++;
		arr =realloc(arr, sizeof(double*) * (count));
		if(!arr) {
			printf("no more memory1\n");
			return -1;
		}
	}
	fclose(fptr);
	arr =realloc(arr, sizeof(double*)* (--count));
	if(!arr) {
		printf("no more memory2\n");
		return -1;
	}
	ftime(&start);
	hull= bruteForce(arr, count, &numHull);
	ftime(&end);
	//hull = arr;
	if (!hull) {
		printf("error\n");
		return -1;
	}
	//sort everything
	divAndConq(hull, numHull);
	//after finished file stop timer and calculate how long it was running then output data
	time = 1000*(end.time -start.time) + (end.millitm -start.millitm);
	printf("Points:\n");
	for(int i=0; i<numHull; i++) {
		printf("%d: (%lf, %lf)\n", i+1, hull[i][0], hull[i][1]);
	}
	printf("~~~~~~~~~~~~~~~~\nrun time = %dms\n", time);
	printf("number of points on the convex hull:%d\n", numHull);
	for(int i=0; i < count; i++) {
		free(arr[i]);
	}
	free(arr);
	free(hull);
	return 1;
}

double** bruteForce(double** arr, int size, int* hullSize) {
	double** hull = NULL;
	double aboveBelow =0;
	double result =0;

	//check if theres a number on both sides of a line created by two points
	for(int i=0; i <= size-1; i++) {
		for(int j=0; j <= size-1; j++) {
			if(i !=j) {
				double a =arr[j][1] - arr[i][1];
				double b =arr[i][0] - arr[j][0];
				double c =arr[i][0] * arr[j][1] -arr[i][1] * arr[j][0];
				//choose a point to compare others to that is not one of the two endpoints
				int m=0;
				while(m==i || m==j) {
					m++;
				}
				aboveBelow = a*(arr[m][0]) + b*(arr[m][1]) -c;
				/*if(j<size-1) {
					aboveBelow = a*arr[j+1][1] + b*arr[j+1][0] - c;
				} else if(i>0){
					aboveBelow = a*arr[0][1] + b*arr[0][0] - c;
				} else {
					aboveBelow = a*arr[1][1] + b*arr[1][0] - c;
				}*/
				if(aboveBelow) { //if the chosen point lies on the same line then the line from the two endpoints should not be there
								//instead there should be a line between both endpoints to the mid point
					int k =0;
					for(k=0; k < size-1; k++) {
						if(k!=i && k!=j) {
							result = a*(arr[k][0]) + b*(arr[k][1]) - c;
							if ((result > 0 && aboveBelow > 0) || (result < 0 && aboveBelow < 0)) { //if they share the same sign then do nothing
							} else { //if they dont then this isnt a extreme point ie theres a point between them or at least one above and below
								k = 0;
								break;
							}
						}
					}
					if (k == size-1) {
						(*hullSize) = (*hullSize) +1;
						hull =realloc(hull, sizeof(double*)* (*hullSize));
						if(!hull) {
							printf("no more memory3\n");
							return NULL;
						}
						hull[*hullSize-1] = arr[i];
						break;
					}
				}//end of if(aboveBelow)
			}
		}//end of for j loop
	}
	return hull;
}

void divAndConq(double** arr, int size) {
	double** arrb;
	double** arrc;
	//if the array size is 1 just return count =0 because no swapping needs to be done
	if(size >1) {
		//make 2 sub arrays and populate them with first and second half of original array
		arrb = malloc (sizeof(double*) * (size/2));
		arrc = malloc (sizeof(double*) * ceil(size/2.0));
		for(int i=0; i < (size/2); i++) {
			arrb[i] = arr[i];
		}
		for(int i=(size/2); i < size; i++) {
			arrc[i-(size/2)] = arr[i];
		}

		//sort the first subarray and count the amount of numbers out of place
		divAndConq(arrb, (size/2));
		//sort the second subarray and count the amount of numbers out of place
		divAndConq(arrc, (int)ceil(size/2.0));

		//set values to use for comparing each subarray
		int i=0, j=0, k=0;
		//make sure that neither subarray goes out of bounds and replace all elements in original array with a sorted version of the sub array ones
		while(i < (size/2) && j < (int)ceil(size/2.0)) {
			//if the first subarray has an element bigger than the second add the one from the second to the original array
			if(arrb[i][0] > arrc[j][0]) {
				arr[k++] = arrc[j++];
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
}