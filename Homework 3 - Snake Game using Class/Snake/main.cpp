// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <unordered_set>
#include "curses.h"
#include "snake.h"

using namespace std;
extern int maxX;
extern int maxY;
extern unordered_set<int>table;

int main() {
	srand((unsigned int)time(NULL));
	ifstream input("gameData.txt");
	int c;
	bool gameEnded = false;
	bool gameTerminated = false;

	initscr();
	cbreak();				// Disable buffering, making input available to the program immediately
	timeout(1000);			// Wait user's input for at most 1s
	noecho();				// Turn off the echos of key strokes
	keypad(stdscr, TRUE);	// Enable input from arrow keys
	curs_set(0); 			// Hide cursor
	getmaxyx(stdscr, maxY, maxX);	// Get max X and Y of the terminal
	bool flag;
	Snake* snake = NewSnake(1, 1, RIGHT);
	Snake* snake2 = NewSnake(maxX >> 1, maxY >> 1, LEFT);
	GenerateNewTarget(snake);
	
	if(input >> flag && flag) {
		int x, y;
		Direction dir;
		input >> (int&)dir >> x >> y ;
		snake->head->x = x;
		snake->tail->y = y;
		snake->dir = dir;
		table.insert(x * maxY + y);
		mvaddch(y, x, '*');
		while (input >> x && x != -1) {
			input >> y;
			Node* temp = new Node;
			temp->x = x;
			temp->y = y;
			snake->head->prevNode = temp;
			snake->head = temp;
			table.insert(x * maxY + y);
			mvaddch(y, x, '*');
		}
		input >> (int&)dir >> x >> y;
		snake2->head->x = x;
		snake2->tail->y = y;
		snake2->dir = dir;
		table.insert(x * maxY + y);
		mvaddch(y, x, '*');
		while (input >> x && x != -1) {
			input >> y;
			Node* temp = new Node;
			temp->x = x;
			temp->y = y;
			snake2->head->prevNode = temp;
			snake2->head = temp;
			table.insert(x * maxY + y);
			mvaddch(y, x, '*');
		}
	}
	else {
		table.insert(1 * maxY + 1);
		table.insert((maxX >> 1) * maxY + (maxY >> 1));
	}
	while (!gameEnded) {
		c = getch();
		switch (c) {
		case KEY_UP:
			SetDir(snake, UP);
			break;
		case KEY_DOWN:
			SetDir(snake, DOWN);
			break;
		case KEY_LEFT:
			SetDir(snake, LEFT);
			break;
		case KEY_RIGHT:
			SetDir(snake, RIGHT);
			break;
		case 'w':
			SetDir(snake2, UP);
			break;
		case 's':
			SetDir(snake2, DOWN);
			break;
		case 'a':
			SetDir(snake2, LEFT);
			break;
		case 'd':
			SetDir(snake2, RIGHT);
			break;

		case 27:
		case 'q':
			gameTerminated = true;
			break;
		default:
			break;
		}

		gameEnded = !MoveSnake(snake) || !MoveSnake(snake2) || gameTerminated;
	}
	if (gameTerminated) {
		ofstream output("gameData.txt", ios::out);
		output << "1\n" << ' ' << snake->dir << ' ';
		Node* temp = snake->tail;
		while (temp) {
			output << temp->x << ' ' << temp->y << ' ';
			temp = temp->prevNode;
		}
		output << "-1\n" << snake2->dir << ' ';
		temp = snake2->tail;
		while (temp) {
			output << temp->x << ' ' << temp->y << ' ';
			temp = temp->prevNode;
		}
		output << "-1\n";
		output.close();
	}
	else {
		ofstream output("gameData.txt", ios::out);
		output << 0;
	}
	DeleteSnake(snake);
	endwin();	// Restores the terminal after Curses activity
}
