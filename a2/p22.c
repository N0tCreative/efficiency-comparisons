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

double** quickHull(double** arr, int size, int* hullSize);
double** quickHullSub(double** arr, int size, int* hullSize, double* r, double* t);
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
	hull= quickHull(arr, count, &numHull);
	ftime(&end);
	//hull = arr;
	if (!hull) {
		printf("error\n");
		return -1;
	}
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

double** quickHull(double** arr, int size, int* hullSize) {
	double** hull = malloc(sizeof(double*) * 2);
	double** tmpHull;
	double** arrB = malloc(sizeof(double*));
	double** arrC = malloc(sizeof(double*));
	double a;
	double b;
	double c;
	int bSize =0;
	int cSize =0;
	int tmpHullSize =0;

	double* right = arr[0];
	double* left = arr[0];

	//find left and rightmost elements
	for(int i=1; i<size; i++) {
		if(arr[i][0] > right[0]) {
			right =arr[i];
		} else if(arr[i][0] < left[0]) {
			left =arr[i];
		}
	}
	hull[0] =left;
	hull[1] =right;
	*hullSize =2;

	a =right[1] -left[1];
	b =left[0] - right[0];
	c = (left[0] * right[1]) - (left[1] * right[0]);

	//for every element not on that line add it to either the lower array or upper array depending on where it is located
	for(int i=0; i< size; i++) {
		double result =a*(arr[i][0]) + b*(arr[i][1]) -c;
		if(arr[i] != right && arr[i] !=left) {
			if(result > 0) {
				bSize++;
				arrB = realloc(arrB, sizeof(double*) * bSize);
				if(!arrB) {
					printf("no more memory\n");
					return NULL;
				}
				arrB[bSize-1] = arr[i];
			} else if(result < 0) {
				cSize++;
				arrC = realloc(arrC, sizeof(double*) * cSize);
				if(!arrC) {
					printf("no more memory\n");
					return NULL;
				}
				arrC[cSize-1] = arr[i];
			}
		}
	}
	/*printf("bsize= %d\n", bSize);
	for(int i=0; i< bSize; i++) {
		printf("B[%d]= %lf, %lf\n", i, arrB[i][0], arrB[i][1]);
	}
	printf("csize= %d\n", cSize);
	for(int i=0; i< cSize; i++) {
		printf("C[%d]= %lf, %lf\n", i, arrC[i][0], arrC[i][1]);
	}*/
	//printf("Lower~~~~~~~~~~\n");
	tmpHull = quickHullSub(arrB, bSize, &tmpHullSize, left, right);
	if(tmpHull) {
		hull =realloc(hull, sizeof(double*) * ((*hullSize) + tmpHullSize));
		for(int i=0; i< tmpHullSize; i++) {
			hull[(*hullSize) + i] = tmpHull[i];
		}
		*hullSize += tmpHullSize;
		free(tmpHull);
	}
	//printf("B hull ptr %p\n", tmpHull);
	free(arrB);
	
	
	tmpHullSize =0;
	//printf("Upper~~~~~~~~~~\n");
	tmpHull = quickHullSub(arrC, cSize, &tmpHullSize, right, left);
	//printf("C hull ptr %p\n", tmpHull);
	if(tmpHull) {
		hull =realloc(hull, sizeof(double*) * ((*hullSize) + tmpHullSize));
		for(int i=0; i< tmpHullSize; i++) {
			hull[(*hullSize) + i] = tmpHull[i];
		}
		*hullSize += tmpHullSize;
		free(tmpHull);
	}
	free(arrC);

	return hull;
}

double** quickHullSub(double** arr, int size, int* hullSize, double* r, double* t) {
	double** tmpHull = NULL;
	double** hull = NULL;
	double** arrB = NULL;
	double** arrC = NULL;
	double* farthest;
	double result;
	double a;
	double b;
	double c;
	double maxDist;
	int bSize =0;
	int cSize =0;
	int tmpHullSize =0;

	if(!arr || size ==0) {
		*hullSize =0;
		return NULL;
	}

	arrB = malloc(sizeof(double*));
	arrC = malloc(sizeof(double*));

	farthest =arr[0];
	a =t[1] -r[1];
	b =r[0] -t[0];
	c =(r[0] * t[1]) - (r[1] * t[0]);
	maxDist =a*(arr[0][0]) + b*(arr[0][1]) -c;


	for(int i=1; i< size; i++) {
		result =a*(arr[i][0]) + b*(arr[i][1]) -c;
		if(result > maxDist) {
			farthest = arr[i];
			maxDist= result;
		}
	}
	hull=malloc(sizeof(double*));
	if(!hull) {
		printf("no more memory\n");
		return NULL;
	}
	hull[0] =farthest;
	(*hullSize)++;

	//add all points to the right of this new hull line to arrB
	a =farthest[1] -r[1];
	b =r[0] -farthest[0];
	c =(farthest[1] * r[0]) - (farthest[0] * r[1]);
	for(int i=0; i< size; i++) {
		if(arr[i] !=farthest && arr[i] != t && arr[i] !=r) {
			if((a*(arr[i][0]) +b*(arr[i][1]) -c) > 0) {
				arrB = realloc(arrB, sizeof(double**) * ++bSize);
				arrB[bSize-1] =arr[i];
			}
		}
	}

	//add all points to the right of this new hull line to arrC
	a =t[1] -farthest[1];
	b =farthest[0] -t[0];
	c =(farthest[0] * t[1]) - (farthest[1] * t[0]);
	for(int i=0; i< size; i++) {
		if(arr[i] !=farthest && arr[i] != t && arr[i] !=r) {
			if((a*(arr[i][0]) +b*(arr[i][1]) -c) > 0) {
				arrC = realloc(arrC, sizeof(double**) * ++cSize);
				arrC[cSize-1] =arr[i];
			}
		}
	}

	/*printf("farthest:%lf, %lf\n", farthest[0], farthest[1]);
	printf("maxdist= %lf\n", maxDist);
	printf("r= %lf, %lf\n", r[0],r[1]);
	printf("t= %lf, %lf\n", t[0],t[1]);
	printf("bSize=%d\n", bSize);
	for(int i=0; i< bSize; i++) {
		printf("innerB[%d]= %lf, %lf\n", i, arrB[i][0], arrB[i][1]);
	}
	printf("cSize=%d\n", cSize);
	for(int i=0; i< cSize; i++) {
		printf("innerC[%d]= %lf, %lf\n", i, arrC[i][0], arrC[i][1]);
	}
	printf("SubB~~~~~~~~~~~~~~\n");*/
	tmpHull = quickHullSub(arrB, bSize, &tmpHullSize, r, farthest);
	//printf("tempHullSzB=%d\n", tmpHullSize);
	//tmpHull =NULL;
	if(tmpHull) {
		hull =realloc(hull, sizeof(double*) * ((*hullSize) + tmpHullSize));
		if(!hull) {
			printf("no more memory\n");
			return NULL;
		}
		for(int i=0; i< tmpHullSize; i++) {
			hull[(*hullSize) + i] = tmpHull[i];
		}
		(*hullSize) += tmpHullSize;
		free(tmpHull);
	}
	free(arrB);
	tmpHullSize =0;

	//printf("SubC~~~~~~~~~~~~~~\n");
	tmpHull = quickHullSub(arrC, cSize, &tmpHullSize, farthest, t);
	//tmpHull =NULL;
	//printf("tempHullSzC=%d\n", tmpHullSize);
	if(tmpHull) {
		hull =realloc(hull, sizeof(double*) * ((*hullSize) + tmpHullSize));
		if(!hull) {
			printf("no more memory\n");
			return NULL;
		}
		for(int i=0; i< tmpHullSize; i++) {
			hull[(*hullSize) + i] = tmpHull[i];
		}
		(*hullSize) += tmpHullSize;
		free(tmpHull);
	}
	free(arrC);

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