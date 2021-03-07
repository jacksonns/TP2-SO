#ifndef QUEUE_H
#define QUEUE_H

struct node{
	int data;
	struct node *next;
};

struct node *front = -1;
struct node *back = -1;

//Funções essenciais 
void enqueue(int frame);
void dequeue();
int frontElement();
void display();

#endif