#include<stdio.h>

int SeqSearch(int array[],int count,int target) {
	int i;
	for(i = count; i>=0; i--) {
		if(array[i] == target)
			return i;
	}
	return i;
}

int BinSearch(int array[],int count,int target)
{
	int low = 0,high = count-1;
	while(low <= high)
	{
		int middle = (low+high)/2;
		if(array[middle] == target)
			return middle;
	 	else if(array[middle] > target)
			high = middle-1;
		else
			low = middle+1;
	}
	
	return -1;
}

int main() {

	int array[] = {0,15,15,37,46,47,54,95};
	int count = sizeof(array)/sizeof(*array);

	int result;//6

	//result = SeqSearch(array,count,54);
	result = BinSearch(array,count,54);

	printf("%i",result);
}
