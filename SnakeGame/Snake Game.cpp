#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <conio.h>

using namespace std;

//Game Over
bool gameOver;

//Game Board
const int height = 20;
const int width = 40;
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void spawnApple()
{
	do {
		fruitX = rand() % width;
		fruitY = rand() % height;
	} while ((fruitX == 0 || fruitX == width - 1 || fruitY == 0 || fruitY == height - 1) ||
		(fruitX == x && fruitY == y));
}

void setup() 
{
	gameOver = false;
	dir = STOP;

	//Print Snake Head in the middle of the Board
	x = width / 2;
	y = height / 2;

	//Randomly Print fruit
	spawnApple();
	score = 0;
}

void draw() {
	system("cls");

	//print top of the border
	for (int i = 0; i < width; i++)
	{
		cout << "#";
	}
	cout << endl;
	////Print side border
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
			{
				cout << "#";
			}
			else if (j == width - 1)
			{
				cout << "#";
			}
			// DRAW S for Snake
			else if (i == y && j == x)
			{
				cout << "S";
			}
			//Draw A for apple
			else if (i == fruitY && j == fruitX)
			{
				cout << "A";
			}
			else 
			{
				std::cout << " ";
			}
		}
		std::cout << endl;
	}

	////print bottom of the border
	for (int i = 0; i < width; i++)
	{
		cout << "#";
	}
	cout << endl;
}

void input() {
	if (_kbhit()) {
		switch (_getch()) 
		{
		case 'w': dir = UP; break;
		case 'a': dir = LEFT; break;
		case 's': dir = DOWN; break;
		case 'd': dir = RIGHT; break;
		case 'x': gameOver = true; break; // Optional: Exit the game
		}
	}
}

void logic()
{

	switch (dir)
	{
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	default:
		break;
	}
}
int main()
{
	setup();
	//while (!gameOver) {
		draw();
		input();
		logic();
	//}

	return 0;
}

