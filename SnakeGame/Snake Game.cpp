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

void getNextAppleCoordinates(int& appleX, int& appleY)
{
	appleX = rand() % (width - 2) + 1;
	appleY = rand() % (height - 2) + 1;
}

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

bool drawApple(int x, int y, int appleX, int appleY)
{
	if (x == appleX && y == appleY)
	{
		print("A");
		return true;
	}

	return false;
}

bool appleCollision(int appleX, int appleY, int snakeX, int snakeY)
{
	if (appleX == snakeX && appleY == snakeY)
	{
		return true;
	}
	return false;
}

void draw(int snakeX, int snakeY, int appleX, int appleY)
{
	setCursorPosition(0, 0);

	// Print side border
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x, y)
				&& !drawSnake(x, y, snakeX, snakeY)
				&& !drawApple(x, y, appleX, appleY))
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

void logic(int& appleX, int& appleY, int& snakeX, int& snakeY)
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
	}

	if (appleCollision(appleX, appleY, snakeX, snakeY))
	{
		getNextAppleCoordinates(appleX, appleY);
	}
}

int main()
{
	setup();

	int snakeX = width / 2;
	int snakeY = height / 2;
	int appleX = 0;
	int appleY = 0;

	getNextAppleCoordinates(appleX, appleY);

	while (!gameOver) 
	{
		input();
		logic(appleX, appleY, snakeX, snakeY);
		draw(snakeX, snakeY, appleX, appleY);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	print("Game Over");
	return 0;
}

