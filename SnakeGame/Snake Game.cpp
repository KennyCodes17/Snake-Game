#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include <iostream>

using namespace std;

//Game Over
bool gameOver;

//Game Board
const int height = 20;
const int width = 40;
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void setup() {
	gameOver = false;
	dir = STOP;

	//Print Snake Head in the middle of the Board
	x = width / 2;
	y = height / 2;

	//Randomly Print fruit
	fruitX = rand() % width;
	fruitY = rand() % height;
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

void input()
{
	std::cin.get();
}

void logic()
{

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

