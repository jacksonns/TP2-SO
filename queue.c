#include "queue.h"

void enqueue(int frame){
	struct node *newNode;
	newNode = (struct node *)malloc(sizeof(struct node));
	newNode->data = frame;
	newNode->next = 0;
	if(back == -1){
		front=back=newNode;
		back->next = front;
	}
	else{
		back->next = newNode;
		back = newNode;
		back->next = front;
	}
}

void dequeue(){
	struct node *temp;
	temp = front;
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else if(front == back){
		front = back = -1;
		free(temp);
	}
	else{
		front = front->next;
		back->next = front;
		free(temp);
	}
}

int frontElement(){
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else{
		return front->data;
	}
}

void display(){
	struct node *temp;
	temp = front;
	if((front == -1)  && (back == -1)){
		printf("queue is empty\n");
	}
	else{
		while (temp->next != front){
			dequeue();
		}
		dequeue();
	}
}