#ifndef SNAKE_H
#define SNAKE_H

using namespace std;

struct Node {
	int x;
	int y;
	Node* prevNode;
	Node();
};

enum Direction{
		UP,DOWN,LEFT,RIGHT
};

struct Snake {
	Node* head;
	Node* tail;
	Direction dir;				// Direction of the snake
};


Snake* NewSnake(int x, int y, Direction direction);
void DeleteSnake(Snake* snake);
void RemoveAtTail(Snake* snake);
void InsertAtHead(Snake* snake, Node* newHead);
void GenerateNewTarget(Snake* snake);			// Generate and draw a new target on the screen
void SetDir(Snake* snake, Direction newDir);	// Set direction
void DrawSnake(Snake* snake);					// Draw the entire snake
bool MoveSnake(Snake* snake);					// Move the snake one step further toward the dir
#endif