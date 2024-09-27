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
bool gamePause;

//Game Board
const int height = 25;
const int width = 45;
//offset the game board
int offsetX = 5; // Shift 5 columns to the right
int offsetY = 3; // Shift 3 rows down


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

void getNextAppleCoordinates(int& appleX, int& appleY, int offsetX, int offsetY)
{
	appleX = rand() % (width - 2) + offsetX + 1;
	appleY = rand() % (height - 2) + offsetY + 1;
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
	if (x == offsetX 
		|| x == width - 1 + offsetX 
		|| y == offsetY 
		|| y == height - 1 + offsetY)
	{
		return true;
	}
	return false;
}


bool checkSnakeSelfCollision(vector<pair<int, int>>& snake)
{
	for (int i = 1; i < snake.size() - 1; i++)
	{
		if (snake[0].first == snake[i].first && snake[0].second == snake[i].second)
		{
			return true;
		}
	}
	return false;
}

void print(std::string s)
{
	std::cout << s;
}

bool drawBorder(int x, int y, int offsetX, int offsetY)
{
	if ((x == offsetX || x == width - 1 + offsetX) ||  
		(y == offsetY || y == height - 1 + offsetY))  
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
		print("o");
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

void draw(int appleX, int appleY, vector<pair<int, int>>& snake, int offsetX, int offsetY)
{
	setCursorPosition(offsetX, offsetY);

	// Print game board
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x + offsetX, y + offsetY, offsetX, offsetY) &&
				!drawSnake(x + offsetX, y + offsetY, snake) &&
				!drawApple(x + offsetX, y + offsetY, appleX, appleY))
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
		case ' ': gamePause = !gamePause; break;
		case 'x': gameOver = true; break; // Optional: Exit the game
		}
	}
}

void gameScore(int appleX, int appleY, vector<pair<int, int>>& snake, int& score)
{

	if (appleCollision(appleX, appleY, snake))
	{
		score = score + 10;
	}
}


void logic(int& appleX, int& appleY, vector<pair<int, int>>& snake, int &score, vector <std::string> playerInitials)
{
	for (size_t i = snake.size() - 1; i > 0; i--)
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
		//generate new apple
		getNextAppleCoordinates(appleX, appleY, offsetX, offsetY);
		//add snake segment
		addSnakeSegment(snake);
		//update game score
		gameScore(appleX, appleY, snake, score);
	}
	//print players name
	setCursorPosition(2, 1);
	print("Player: ");
	//std::cout<< playerInitials;

	//print game score
	setCursorPosition(35,1); // Move cursor below the rectangle
	print("SCORE: ");
	std::cout << score;
}

vector <std::string> collectUserName()
{
	std::string input;
	vector <std::string> initials;

	// Print the stylized message
	print(" SNAKESNAKESNAKESNAKESNAKESNAKESNAKES\n");
	print(" N                                  N\n");
	print(" A      Type in your initials:      A\n");
	print(" K                                  K\n");
	print(" EKANSEKANSEKANSEKANSNAKESNAKESNAKESN\n");

	setCursorPosition(11 + offsetX, 3);

	// Get the initials input from the user
	std::cin >> input;
	initials.push_back(input);

	// Clear the area below or reposition the cursor for the next message
	setCursorPosition(6, 8 ); // Move cursor below the rectangle
	print("Press 'Enter' to play: ");

	// Wait for the user to press Enter
	while (true)
	{
		if (_getch() == '\r') // Check for Enter key
		{
			break;
		}
	}

	return initials;
}


void splashScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set the color to your desired color, e.g., Bright Green
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		print("                                                   \n");
		print("     ##############################################\n");
		print("     #                                            #\n");
		print("     #                                            #\n");
		print("     #    sss   nn   nn     a     kk   kk  eeeee  #\n");
		print("     #   sssss  nn   nn    aaa    kk  kk   eeeee  #\n");
		print("     #  ss      nnn  nn   a   a   kk kk    e      #\n");
		print("     #   sssss  nn n nn  aaaaaaa  kKKK     eeee   #\n");
		print("     #       ss nn  nnn  aa   aa  kK  Kk   ee     #\n");
		print("     #   sssss  nn   nn  aa   aa  kK   Kk  eeeee  #\n");
		print("     #    sss   nn   nn  aa   aa  kk    kk eeeee  #\n");
		print("     #                                            #\n");
		print("     #       gggg       aa     mm   mm  eeeee     #\n");
		print("     #     gg   gg     aaaa    mmm mmm  eeeee     #\n");
		print("     #    gg          a    a   mm m mm  e         #\n");
		print("     #    gg   ggg   aaaaaaaa  mm   mm  eeee      #\n");
		print("     #    gg    gg   aa    aa  mm   mm  ee        #\n");
		print("     #     gg   gg   aa    aa  mm   mm  eeeee     #\n");
		print("     #       ggggg   aa    aa  mm   mm  eeeee     #\n");
		print("     #                                            #\n");
		print("     #                                            #\n");
		print("     #                By: Kenny G                 #\n");
		print("     #                                            #\n");
		print("     #                                            #\n");
		print("     #         'Press Enter to Continue'          #\n");
		print("     #                                            #\n");
		print("     ##############################################\n");

}

int main()
{
	//Generate different seed
	srand(static_cast<unsigned int>(time(0)));
	//Snake Vector
	vector<pair<int, int>> snake;
	
	//player initials
	vector <std::string> playerInitials;

	setup(snake);

	int snakeX = width / 2;
	int snakeY = height / 2;
	int appleX = 0;
	int appleY = 0;
	bool beginGame = true;
	char keyEnter;
	int score = 0;

	getNextAppleCoordinates(appleX, appleY, offsetX, offsetY);

	while (beginGame)
	{
		//display splash screen
		splashScreen();
		//wait for user input to continue
		keyEnter = _getch();
		//check for valid key
		if (keyEnter == ' ' || keyEnter == '\r')
		{
			//clear screen
			system("cls");
			//Collect player Initials
			playerInitials = collectUserName();
			//clear screen again
			system("cls");
			beginGame = false;
		}
		//check if user enters ESC -- End Game
		else if (keyEnter == 27)
		{
			gameOver = true;
		}
	}

	while (!gameOver) 
	{
		input();
		if (!gamePause)
		{
			//This removes the Pause Message
			setCursorPosition(offsetX, height + offsetY + 1);
			print("                                                 ");
			logic(appleX, appleY, snake, score, playerInitials);
			draw(appleX, appleY, snake, offsetX, offsetY);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));

			if (checkSnakeSelfCollision(snake))
			{
				gameOver = true;
			}
		}
		else
		{
			//Pause game message
			setCursorPosition(offsetX, height + offsetY + 1);
			print(" Game has been Pause: Press Space Bar to continue");
		}
	}

	print("Game Over");
	return 0;
}

