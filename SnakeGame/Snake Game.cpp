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

Direction dir = STOP;

void setup()
{
	hideCursor();
	gameOver = false;
	dir = STOP;

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

bool checkBorder(int x, int y)
{
	if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
	{
		return true;
	}
	return false;
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

void draw(int snakeX, int snakeY) 
{
	setCursorPosition(0, 0);

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

void logic(int &snakeX, int &snakeY)
{
	switch (dir)
	{
		case UP:
			snakeY--;
			break;
		case DOWN:
			snakeY++;
			break;
		case LEFT:
			snakeX--;
			break;
		case RIGHT:
			snakeX++;
			break;
		default:
			break;
	}
	
	if (checkBorder(snakeX, snakeY))
	{
		gameOver = true;
		print("Game Over");
	}
}

int main()
{
	setup();

	int snakeX = width / 2;
	int snakeY = height / 2;

	while (!gameOver) {
		draw(snakeX, snakeY);
		input();
		logic(snakeX, snakeY);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return 0;
}

