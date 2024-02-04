#include<stdio.h>
#include<stdlib.h>

typedef struct node {
	int adjvex;
	struct node *next;
} EdgeNode;

typedef struct vnode {
	char vertex;
	EdgeNode *link;
} VertexNode,Adjlist[100];
typedef Adjlist ALGraph;

void CreateGraph(ALGraph graph,int n,int e,char * str) {
	int i;
	for(i=0; i<n; i++) {
		char value = str[i];

		graph[i].vertex = value;
		graph[i].link = NULL;
	}


	for(i=0; i<e; i++) {
		char a = str[n+(i*5)+1],b = str[n+(i*5)+3];
		int origin = a - '0',destination = b - '0';

		EdgeNode* edge = (EdgeNode*)malloc(sizeof(EdgeNode));
		edge->adjvex = destination;
		edge->next = graph[origin].link;
		graph[origin].link = edge;

		edge = (EdgeNode*)malloc(sizeof(EdgeNode));
		edge->adjvex = origin;
		edge->next = graph[destination].link;
		graph[destination].link = edge;
	}
}

int visited[100];
void DFS(ALGraph graph,int i) {
	printf("v%i ",i);
	visited[i] = 1;

	EdgeNode* p = graph[i].link;
	while(p != NULL) {
		if(visited[p->adjvex] != 1)
			DFS(graph,p->adjvex);

		p = p->next;
	}
}

void BFS(ALGraph graph,int i) {
	int queue[100];
	int front = 0,rear = 0;

	queue[rear] = i;
	rear++;
	visited[i] = 1;

	while(front != rear) {
		int index = queue[front];
		front++;

		printf("v%i ",index);
		
		EdgeNode* p = graph[index].link;
		while(p != NULL) {
			if(visited[p->adjvex] != 1) {
				queue[rear] = p->adjvex;
				rear++;
				visited[p->adjvex] = 1;
			}

			p = p->next;
		}
	}
}

int main() {
	ALGraph graph;
	CreateGraph(graph,9,10,"012345678(6,7)(4,8)(4,6)(3,6)(2,5)(1,4)(1,2)(0,4)(0,3)(0,1)");
	//DFS(graph,0);
	BFS(graph,0);

	int i;
	for(i = 0; i<9; i++) {
		printf("\nv%i ",i);
		EdgeNode* p = graph[i].link;
		while(p != NULL) {
			printf("v%i ",p->adjvex);
			p = p->next;
		}
	}
}
