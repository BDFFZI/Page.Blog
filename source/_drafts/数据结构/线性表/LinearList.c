#include<stdio.h>
#include<stdlib.h>

#define ListSize 100
typedef int DataType;
typedef struct {
	DataType data[ListSize];
	int length;
} SeqList;

void InitList(SeqList* seqList) {
	seqList->length = 0;
}

void InsertList(SeqList* seqList,int index,DataType value) {
	if(index < 0 || index > seqList->length)
		printf("����Խ��");

	if(seqList->length == ListSize)
		printf("������������");

	int i;
	for( i = seqList->length; i>index; i-- ) {
		seqList->data[i] = seqList->data[i-1];
	}

	seqList->data[i] = value;
	seqList->length++;
}

void DeleteList(SeqList* list,int index) {
	if(index<0 || index>= list->length)
		printf("����Խ��");

	int i;
	for( i = index; i< list->length-1; i++) {
		list->data[i] = list->data[i+1];
	}
	list->length--;
}

//���542 
int main() {
	SeqList list;
	InitList(&list);

	InsertList(&list,0,1);
	InsertList(&list,1,2);
	InsertList(&list,0,3);
	InsertList(&list,2,4);
	DeleteList(&list,1);
	InsertList(&list,1,5);
	DeleteList(&list,0);

	int i;
	for( i=0; i<list.length; i++) {
		printf("%d ",list.data[i]);
	}
}
