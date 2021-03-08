#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

struct node{
	int data;
	struct node *next;
};

static struct node *front = -1;
static struct node *back = -1;

//Funções essenciais 
void enqueue(int frame);
void dequeue();
struct node *getFront();
int frontElement();
void freeQueue();

#endif