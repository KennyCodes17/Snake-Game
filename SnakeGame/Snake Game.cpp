#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <windows.h>

using namespace std;

//Game Over
bool gameOver;

//Game Board
const int height = 20;
const int width = 40;
//int x, y, fruitX, fruitY, score;

enum Direction 
{ 
	STOP = 0, 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN 
};

void hideCursor();

Direction dir = RIGHT;
//
//void spawnApple()
//{
//	do 
//	{
//		fruitX = rand() % width;
//		fruitY = rand() % height;
//	} 
//	while ((fruitX == 0 || fruitX == width - 1 || fruitY == 0 || fruitY == height - 1) ||
//		(fruitX == x && fruitY == y));
//}

void setup()
{
	hideCursor();
	gameOver = false;
	dir = STOP;

	////Print Snake Head in the middle of the Board
	//x = width / 2;
	//y = height / 2;

	////Randomly Print fruit
	//spawnApple();
	//score = 0;
}

void setCursorPosition(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(hConsole, position);
}

void hideCursor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE; // Set the cursor visibility to false
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void print(std::string s)
{
	std::cout << s;
}

bool drawBorder(int x, int y)
{
	if (x == 0 
		|| y == 0
		|| x == width - 1
		|| y == height - 1)
	{
		print("#");
		return true;
	}

	return false;
}

bool drawSnake(int x, int y, int snakeX, int snakeY)
{
	if (x == snakeX && y == snakeY)
	{
		print("s");
		return true;
	}

	return false;
}

void draw() 
{
	setCursorPosition(0, 0);

	int snakeX = width / 2;
	int snakeY = height / 2;

	// Print side border
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x, y) && !drawSnake(x, y, snakeX, snakeY))
			{
				print(" ");
			}
		}
		std::cout << endl;
	}

	cout << endl;
}

void input() 
{
	if (_kbhit()) 
	{
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
			//y--;
			break;
		case DOWN:
			//++;
			break;
		case LEFT:
			//x--;
			break;
		case RIGHT:
			//x++;
			break;
		default:
			break;
	}
}

int main()
{
	setup();
	while (!gameOver) {
		draw();
		input();
		logic();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

