#include<stdio.h>
#include<stdlib.h>

typedef struct node {
	char data;
	struct node *lchild,*rchild;
} BinTNode;
typedef BinTNode *BinTree;

BinTree CreateTree(char *str) {
	BinTNode* binTnodes[100];
	int top = -1;

	BinTNode* root = NULL;
	BinTNode* currentRoot;

	int i,j;
	char ch;
	for(i=0; (ch=str[i])!='\0'; i++){
		switch(ch) {
			case '(':
				top++;
				binTnodes[top] = currentRoot;
				j = 0;
				break;
			case ')':
				top--;
				break;
			case ',':
				j = 1;
				break;
			default:
				currentRoot = (BinTNode*)malloc(sizeof(BinTNode));
				currentRoot->data = ch;
				currentRoot->lchild = NULL;
				currentRoot->rchild = NULL;
				if(root == NULL) {
					root = currentRoot;
				} else {
					if(j == 0) binTnodes[top]->lchild = currentRoot;
					else binTnodes[top]->rchild = currentRoot;
				}
				break;
		}
	}
	
	return root;
}

void Preorder(BinTree bt)
{
	if(bt==NULL)
		return;
	printf("%c ",bt->data);
	Preorder(bt->lchild);
	Preorder(bt->rchild);
}

void Inorder(BinTree bt)
{
	if(bt==NULL)
		return;
	Inorder(bt->lchild);
	printf("%c ",bt->data);
	Inorder(bt->rchild);
}

void Postorder(BinTree bt)
{
	if(bt==NULL)
		return;
	Postorder(bt->lchild);
	Postorder(bt->rchild);
	printf("%c ",bt->data);
}

int main() {
	BinTree binTree = CreateTree("(A(B(,D(E,F)),C))");
	Postorder(binTree);
}
