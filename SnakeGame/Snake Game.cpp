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
	appleX = rand() % (width - offsetX - 2) + offsetX + 1;
	appleY = rand() % (height - offsetY - 2) + offsetY + 1;

}

void setup(vector<pair<int, int>>& snake)
{
	hideCursor();
	gameOver = false;
	dir = STOP;

	snake.push_back(make_pair(width / 2 , height / 2 ));
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
	if (x < 2  || x >= width - 1 || y < 2 || y >= height - 2)
	{
		return true; // Out of bounds
	}
	return false; // Inside bounds
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

bool drawBorder(int x, int y)
{
	if ((x == 0 || x == width - 1) ||  
		(y == 0 || y == height - 1))  
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
	if (x == appleX && y == appleY 
		&& x > 0 
		&& x < width - 1 
		&& y > 0 
		&& y < height - 1)
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
		setCursorPosition(offsetX, offsetY + y);

		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x, y) &&
				!drawSnake(x, y, snake) &&
				!drawApple(x, y, appleX, appleY))
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

void gameScore(int& score)
{

	score = score + 10;
	setCursorPosition(0, 0);

}

//speed of the snake increases if the socre is a multiple of 50
int adjustSpeedForScore(int score, int currentSpeed, int minSpeed) {
	// Increase speed every multiple of 50, decreasing sleep time
	if (score % 50 == 0 && score != 0 && currentSpeed > minSpeed) {
		currentSpeed -= 50; // Reduce delay by 50ms
	}
	return currentSpeed; // Return updated speed
}

void logic(int& appleX, int& appleY, vector<pair<int, int>>& snake, int& score, vector <std::string> playerInitials, int& currentSpeed, int minSpeed)
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

	//print players name
	setCursorPosition(5, 1);
	print("Player: ");
	std::cout << playerInitials[0];

	//print game score
	setCursorPosition(40, 1); // Move cursor below the rectangle
	print("SCORE: ");
	std::cout << score;

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
		gameScore(score);
		//adjust the speed based on score
		currentSpeed = adjustSpeedForScore(score, currentSpeed, minSpeed);
	}
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

void gameOverScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	print("                                                   \n");
	print("     ########################################\n");
	print("     #                                        #\n");
	print("     #                                        #\n");
	print("     #   gggggg      a    mm    mm  eeeee     #\n");
	print("     #  gg    gg    a a   mmm  mmm  eeeee     #\n");
	print("     #  gg         a   a  mm mm mm  e         #\n");
	print("     #  gg  gggg  aaaaaaa mm mm mm  eeeee     #\n");
	print("     #  gg    gg  aa   aa mm mm mm  ee        #\n");
	print("     #   gggggg   aa   aa mm    mm  eeeee     #\n");
	print("     #                                        #\n");
	print("     #    ooooo  vv   vv  eeeee  rrrrrr       #\n");
	print("     #   oo   oo vv   vv  eeeee  rr  rrr      #\n");
	print("     #   oo   oo vv   vv  e      rrrrr        #\n");
	print("     #   oo   oo  vv vv   eeee   rr rr        #\n");
	print("     #   oo   oo   vvv    ee     rr  rr       #\n");
	print("     #    ooooo    vvv    eeeee  rr   rr      #\n");
	print("     #                                        #\n");
	print("     #                                        #\n");
	print("     #                                        #\n");
	print("     #     Press Enter to Exit the Game       #\n");
	print("     #                                        #\n");
	print("     #########################################\n");
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

//speed of the snake increases if user holds down a key
void adjustSpeedForKeyHold(char lastKeyPressed, char currentKey, int& currentSpeed, int normalSpeed) {
	if (currentKey == lastKeyPressed) {
		currentSpeed = normalSpeed / 2; 
	}
	else {
		currentSpeed = normalSpeed;
	}
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
	int speed = 800;
	int minSpeed = 100;

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
			print("                                                   ");
			logic(appleX, appleY, snake, score, playerInitials, speed, minSpeed);
			draw(appleX, appleY, snake, offsetX, offsetY);
			std::this_thread::sleep_for(std::chrono::milliseconds(speed));

			if (checkSnakeSelfCollision(snake))
			{
				gameOver = true;
			}
		}
		else
		{
			//Pause game message
			setCursorPosition(offsetX, height + offsetY + 1);
			print(" Game has been Paused: Press Space Bar to continue");
		}
	}

	system("cls");
	gameOverScreen();
	return 0;
}

