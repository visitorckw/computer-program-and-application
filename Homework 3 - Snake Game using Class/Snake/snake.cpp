#include "snake.h"
#include "curses.h"
#include <stdlib.h>
#include <iostream>
#include <unordered_set>

using namespace std;

int targetX;
int targetY;
int maxX;
int maxY;
unordered_set<int>table;

Node::Node() {
	prevNode = NULL;
}

Snake* NewSnake(int x, int y, Direction direction) {
	// Create snake's head at the specified position
	Snake* snake = new Snake;
	snake->head = new Node;
	snake->head->x = x;
	snake->head->y = y;
	snake->head->prevNode = NULL;
	snake->tail = snake->head;
	
	snake->dir = direction;

	return snake;
}

void DeleteSnake(Snake* snake) {
	while(snake->tail != NULL) {
		Node* temp = snake->tail;
		snake->tail = snake->tail->prevNode;
		delete temp;
	}
	delete snake;
}

void SetDir(Snake* snake, Direction dir) {
	snake->dir = dir;
}

void RemoveAtTail(Snake* snake) {
	table.erase(snake->tail->x * maxY + snake->tail->y);
	Node* temp = snake->tail;
	snake->tail = snake->tail->prevNode;
	delete temp;
}

void InsertAtHead(Snake* snake, Node* newHead) {
	snake->head->prevNode = newHead;
	snake->head = newHead;
	table.insert(snake->head->x * maxY + snake->head->y);
}


bool MoveSnake(Snake* snake) {
	Node* newHead = new Node;
	
	*newHead = *(snake->head); // copy the head
	
	// Compute position of the new head
	switch(snake->dir) {
	case UP:
		newHead->y--;
		break;
	case DOWN:
		newHead->y++;
		break;
	case LEFT:
		newHead->x--;
		break;
	case RIGHT:
		newHead->x++;
		break;
	}
	
	// Check if we've hit the wall
	if (newHead->x < 0 || newHead->x >= maxX || newHead->y < 0 || newHead->y >= maxY)
		return false;
	//Check if hit the other snake
	if (table.count(newHead->x * maxY + newHead->y))
		return false;
	// Draw the new head
	InsertAtHead(snake, newHead);
	mvaddch(newHead->y, newHead->x, '*');
	
	// Clear the tail if we haven't reached the target
	if (targetX !=newHead->x || targetY != newHead->y) {
		mvaddch(snake->tail->y, snake->tail->x, ' ');
		RemoveAtTail(snake);
	} else {
		GenerateNewTarget(snake);
	}
	
	return true;
}

void GenerateNewTarget(Snake* snake) {	
	targetX = rand() % maxX;
	targetY = rand() % maxY;

	// Should check to make sure the new target does not overlap with the snake body
	mvaddch(targetY, targetX, '$');
}
