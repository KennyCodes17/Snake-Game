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

void setup(vector<pair<int, int>>& snake)
{
	hideCursor();
	gameOver = false;
	dir = STOP;

	snake.push_back(make_pair(width / 2, height / 2));

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

bool checkSnakeSelfCollision(vector<pai<int, int>>& snake)
{

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

bool drawSnake(int x, int y, vector<pair<int, int>>& snake)
{
	for (int i = 0; i < snake.size(); i++)
	{
		if (x == snake[i].first && y == snake[i].second)
		{
			print("s");
			return true;
		}
	}
	return false;
}

void addSnakeSegment(vector<pair<int, int>>& snake)
{
	snake.push_back(snake.back());
}

bool drawApple(int x, int y, int appleX, int appleY)
{
	if (x == appleX && y == appleY)
	{
		print("a");
		return true;
	}

	return false;
}

bool appleCollision(int appleX, int appleY, vector<pair<int, int>>& snake)
{
	if (appleX == snake[0].first && appleY == snake[0].second)
	{
		return true;
	}
	return false;
}

void draw(int appleX, int appleY, vector<pair<int, int>>& snake)
{
	setCursorPosition(0, 0);

	// Print side border
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x, y)
				&& !drawSnake(x, y, snake)
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

void logic(int& appleX, int& appleY, vector<pair<int, int>>& snake)
{
	for (int i = snake.size() - 1; i > 0; i--)
	{
		snake[i] = snake[i - 1];
	}

	switch (dir)
	{
	//Y coordinate stored in second element of the vector
	case UP:
		snake[0].second--;
		break;
	//Y coordinate stored in second element of the vector
	case DOWN:
		snake[0].second++;
		break;
	//X coordinate stored in first element of the vector
	case LEFT:
		snake[0].first--;
		break;
	//X coordinate stored in first element of the vector
	case RIGHT:
		snake[0].first++;
		break;
	default:
		break;
	}

	//END GAME if collides with border
	if (checkBorder(snake[0].first, snake[0].second))
	{
		gameOver = true;
	}

	if (appleCollision(appleX, appleY, snake))
	{
		getNextAppleCoordinates(appleX, appleY);
		addSnakeSegment(snake);
		cout << "Apple eaten! New segment added. Total segments: " << snake.size() << endl;
	}
}

int main()
{
	//Snake Vector
	vector<pair<int, int>> snake;

	setup(snake);

	int snakeX = width / 2;
	int snakeY = height / 2;
	int appleX = 0;
	int appleY = 0;

	getNextAppleCoordinates(appleX, appleY);

	while (!gameOver) 
	{
		input();
		logic(appleX, appleY, snake);
		draw(appleX, appleY, snake);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	print("Game Over");
	return 0;
}

