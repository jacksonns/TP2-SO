#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

void enqueue(int frame){
	struct node *newNode;
	newNode = (struct node *)malloc(sizeof(struct node));
	newNode->data = frame;
	newNode->next = 0;
	if(_back == -1){
		_front=_back=newNode;
		_back->next = _front;
	}
	else{
		_back->next = newNode;
		_back = newNode;
		_back->next = _front;
	}
}

void dequeue(){
	struct node *temp;
	temp = _front;
	if((_front == -1)  && (_back == -1)){
		printf("queue is empty\n");
	}
	else if(_front == _back){
		_front = _back = -1;
		free(temp);
	}
	else{
		_front = _front->next;
		_back->next = _front;
		free(temp);
	}
}

int frontElement(){
	if((_front == -1)  && (_back == -1)){
		printf("queue is empty\n");
	}
	else{
		return _front->data;
	}
}

void display(){
	struct node *temp;
	temp = _front;
	if((_front == -1)  && (_back == -1)){
		printf("queue is empty\n");
	}
	else{
		while (temp->next != _front){
			dequeue();
		}
		dequeue();
	}
}