// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <ctime>
#include "curses.h"

using namespace std;

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

struct SingleNodeSnake {
	int x;
	int y;
	SingleNodeSnake * front, *back;
	Direction dir;
};
SingleNodeSnake* snake = new SingleNodeSnake, * snakeTail = snake;
void insertHead() {
	SingleNodeSnake *temp = new SingleNodeSnake;
	*temp = *snake;
	temp->front = temp;
	temp->back = snake;
	snake->front = temp;
	switch (temp->dir) {
	case UP:
		temp->y--;
		break;
	case DOWN:
		temp->y++;
		break;
	case LEFT:
		temp->x--;
		break;
	case RIGHT:
		temp->x++;
		break;
	}
	snake = temp;
	mvaddch(snake->y, snake->x, '*');
}
void removeTail() {
	SingleNodeSnake* temp = snakeTail;
	mvaddch(temp->y, temp->x, ' ');
	snakeTail = snakeTail->front;
	snakeTail->back = snakeTail;
	delete temp;
}

void move() {
	// Clear '*' at the previous position
	/*mvaddch(snake.y, snake.x, ' ');

	// Update snake's position
	switch (snake.dir) {
	case UP:
		snake.y--;
		break;
	case DOWN:
		snake.y++;
		break;
	case LEFT:
		snake.x--;
		break;
	case RIGHT:
		snake.x++;
		break;
	}

	// Draw '*' at the new position
	mvaddch(snake.y, snake.x, '*');*/
	insertHead();
	removeTail();
}

int main() {
	int c, maxX, maxY;
	bool terminated = false;
	srand((unsigned int)time(NULL));
	pair<int, int>food;

	// Set the initial position and moving direction
	snake->x = 1;
	snake->y = 1;
	snake->front = snake->back = snake;
	snake->dir = RIGHT;

	initscr();
	cbreak();						// Disable buffering, making input available to the program immediately
	timeout(500);					// Wait user's input for at most 1s
	keypad(stdscr, TRUE);			// Enable input from arrow keys
	curs_set(0); 					// Hide cursor
	getmaxyx(stdscr, maxY, maxX);	// Get max X and Y of the terminal
	printw("%d %d", maxX, maxY);

	// Draw the single headed snake
	mvaddch(snake->y, snake->x, '*');

	food.first = rand() % maxY;
	food.second = rand() % maxX;
	mvaddch(food.first, food.second, '$');


	while (!terminated) {
		c = getch();

		switch (c) {
		case KEY_UP:
			snake->dir = UP;
			break;
		case KEY_DOWN:
			snake->dir = DOWN;
			break;
		case KEY_LEFT:
			snake->dir = LEFT;
			break;
		case KEY_RIGHT:
			snake->dir = RIGHT;
			break;
		case 27: // ESC
		case 'q':
			terminated = true;
			break;
		default:
			break;
		}
		if (snake->y == food.first and snake->x == food.second) {
			//mvaddch(food.first, food.second, ' ');
			insertHead();
			food.first = rand() % maxY;
			food.second = rand() % maxX;
			mvaddch(food.first, food.second, '$');
		}
		move();
	}
	endwin();	// Restores the terminal after Curses activity
}
