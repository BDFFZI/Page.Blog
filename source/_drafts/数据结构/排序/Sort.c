#include<stdio.h>

void InsertSort(int array[],int count) {
	int i,j;
	for(i = 1; i<count; i++) {
		if(array[i] <array[i-1]) {
			int value = array[i];
			for(j = i-1; j>=0 && array[j] > value; j--)
				array[j+1] = array[j];

			array[j+1] = value;
		}
	}
}

void BubbleSort(int array[],int count) {
	int i,j,flag;
	for(i = 1; i<count; i++) {
		flag = 0;
		for(j = count-1; j>= i; j--) {
			if(array[j] < array[j-1]) {
				flag = 1;
				int value = array[j];
				array[j] = array[j-1];
				array[j-1] = value;
			}
		}
		if(flag == 0)
			return;
	}
}

void StraightSelectSort(int array[],int count) {
	int i,j,minValue,minIndex;

	for(i = 0; i<count; i++) {
		minValue = array[i];
		for(j = i+1; j<count; j++) {
			if(array[j] < minValue) {
				minValue = array[j];
				minIndex = j;
			}
		}

		array[minIndex] = array[i];
		array[i] = minValue;
	}
}

int main() {
	int array[] = {15,37,95,54,15,46,0,47};	//0 15 15 37 46 47 54 95
	int count = sizeof(array)/sizeof(*array);

	//InsertSort(array,count);
	//BubbleSort(array,count);
	StraightSelectSort(array,count);


	int i;
	for(i=0; i<count; i++) {
		printf("%d ",array[i]);
	}
}
