#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime> 
#include <conio.h>
#include <chrono>
#include <thread>
#include <windows.h>
#include "SpeedController.h"

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

void setup(vector<pair<int, int>>& snake)
{
	hideCursor();
	gameOver = false;
	dir = STOP;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set the entire game screen to green
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

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
	if (x < 2  || x >= width - 1 || y < 2 || y >= height - 1)
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
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Set the border color to brown
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		print("#");

		// Reset the color back to green for other elements
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

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
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			// Set the snake color to white
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			print("s");

			// Reset the color back to green for other elements
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

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

void getNextAppleCoordinates(int& appleX, int& appleY, int offsetX, int offsetY)
{
	appleX = rand() % (width - offsetX - 2) + offsetX + 1;
	appleY = rand() % (height - offsetY - 2) + offsetY + 1;
}

void getNextBadAppleCoordinates(int& badAppleX, int& badAppleY, int offsetX, int offsetY)
{
		badAppleX = rand() % (width - offsetX - 2) + offsetX + 1;
		badAppleY = rand() % (height - offsetY - 2) + offsetY + 1;
}

bool drawBadApple(int x, int y, int badAppleX, int badAppleY)
{
	if (x == badAppleX && y == badAppleY
		&& x > 0
		&& x < width - 1
		&& y > 0
		&& y < height - 1)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Set the text color to red
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

		// Draw the bad apple
		print("X");

		// Reset the text color back to green
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

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

bool badAppleCollision(int badAppleX, int badAppleY, vector<pair<int, int>>& snake)
{
	if (badAppleX == snake[0].first && badAppleY == snake[0].second)
	{
		return true;
	}
	return false;
}

void draw(int appleX, int appleY, vector<pair<int, int>>& snake, int offsetX, int offsetY, int badAppleX, int badAppleY)
{
	setCursorPosition(offsetX, offsetY);

	// Print game board
	for (int y = 0; y < height; y++)
	{
		setCursorPosition(offsetX, offsetY + y);

		for (int x = 0; x < width; x++)
		{
			if (!drawBorder(x, y) 
				&& !drawSnake(x, y, snake) 
				&& !drawBadApple(x, y, badAppleX, badAppleY)
				&& !drawApple(x, y, appleX, appleY))
			{
				print(" ");
			}
		}
		std::cout << endl;
	}
	cout << endl;
}

int adjustSpeedForScore(int score, int& currentSpeed, int minSpeed) {

	// Decrease speed by 50ms every time the score is a multiple of 10
	if (score % 10 == 0 && score != 0 && currentSpeed > minSpeed) {
		currentSpeed -= 50; // Reduce delay by 50ms
		std::cout << "Speed adjusted! New speed: " << currentSpeed << "ms" << std::endl; // Debug print
	}

	// Ensure the current speed does not drop below minSpeed (e.g., 50ms)
	if (currentSpeed < minSpeed) {
		currentSpeed = minSpeed; // Set currentSpeed to minSpeed
		std::cout << "Speed is now at minimum allowed: " << currentSpeed << "ms" << std::endl;
	}

	return currentSpeed;
}

int adjustSpeedForKeyHold(int speed, int minSpeed, int increasedSpeed, int& speedBeforeKeyPress)
{
	// Check if any of the directional keys ('W', 'A', 'S', 'D') are being held down
	if ((GetKeyState('W') & 0x8000) || (GetKeyState('A') & 0x8000) ||
		(GetKeyState('S') & 0x8000) || (GetKeyState('D') & 0x8000))
	{
		// Save the original speed if this is the first time holding the key
		if (speedBeforeKeyPress == speed) {
			speedBeforeKeyPress = speed; // Store the original speed before speeding up
		}

		// Return the increased speed while a directional key is held
		return increasedSpeed;
	}

	// If no directional key is being held, revert to the original speed
	return speedBeforeKeyPress;
}

char input()
{
	if (_kbhit())
	{
		char key = _getch();
		switch (key)
		{
		case 'w': dir = UP; break;
		case 'a': dir = LEFT; break;
		case 's': dir = DOWN; break;
		case 'd': dir = RIGHT; break;
		case ' ': gamePause = !gamePause; break;
		case 'x': gameOver = true; break; // Optional: Exit the game
		}
		return key;  // Return the key pressed
	}
	return '\0'; // Return null character if no key is pressed
}

void gameScore(int& score)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//set color
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	score = score + 10;
	setCursorPosition(0, 0);
}

void logic(int& appleX, int& appleY, vector<pair<int, int>>& snake, int& score, vector <std::string> playerInitials, int& currentSpeed, int minSpeed, int &badAppleX, int &badAppleY, bool &badAppleGenerated, int &badAppleCounter)
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
	//set teal color
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	//print players name
	setCursorPosition(5, 1);
	print("Player: ");
	std::cout << playerInitials[0];

	//Print bad apple counter
	setCursorPosition(5, 2);
	print("Bad apples eaten: ");
	std::cout << badAppleCounter;

	//print game score
	setCursorPosition(40, 1); // Move cursor below the rectangle
	print("SCORE: ");
	std::cout << score;

	setCursorPosition(40, 2); // Move the cursor just below the score
	print("SPEED for key holding: ");
	std::cout << currentSpeed << "ms"; // Print the current speed

	//END GAME if collides with border
	if (checkBorder(snake[0].first, snake[0].second))
	{
		gameOver = true;
	}
	if (appleCollision(appleX, appleY, snake))
	{
		//generate new apple
		getNextAppleCoordinates(appleX, appleY, offsetX, offsetY);
		//generate bad apple
		addSnakeSegment(snake);
		//update game score
		gameScore(score);
		//adjust the speed based on score
		currentSpeed = adjustSpeedForScore(score, currentSpeed, minSpeed);
	}
	// check score has reached a multiple of 50
	// if so, generate a bad apple and await collision
	if (score % 10 == 0 && score != 0 && !badAppleGenerated)
	{
		getNextBadAppleCoordinates(badAppleX, badAppleY, offsetX, offsetY);
		badAppleGenerated = true;  //No new bad apple is generated until collision
	}
	if (badAppleCollision(badAppleX, badAppleY, snake))
	{
		badAppleCounter += 1;
		badAppleGenerated = false;  // Reset to allow new bad apple generation after another apple is eaten
		//check if 3 bad apples have been eaten
		if (badAppleCounter >= 3)
		{
			gameOver = true;  // End the game if 3 bad apple collisions occurred
			// Or any other logic you want to implement
		}
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
	print("Press 'Enter' to continue: ");

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
	print("     #  Press Enter to View your Final Score  #\n");
	print("     #                                        #\n");
	print("     #########################################\n");
}

void highScoreDisplay(vector <std::string> playerInitials, int& score)
{
	print(" Final Game Score: ");
	std::cout << score;
}


void printGameRules()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	print("                             SNAKE GAME RULES\n");
	print("                             ----------------\n");
	print("                                                                                 \n");
	print("     Press or hold 'A', 'W', 'S', 'D' to control the direction of the snake\n");
	print("                                                                                 \n");
	print("                     Avoid all contact with the walls\n");
	print("                                                                                 \n");
	print("                    Do not eat more than 3 bad apples\n");
	print("                                                                                 \n");
	print("                    Press Space Bar to pause the game\n");
	print("                                                                                 \n");
	print("                         Good Luck, Have Fun!\n");
	print("                                                                                 \n");
}


void splashScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set the color to your desired color, e.g., Bright Green
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
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
	int badAppleX = 0;
	int badAppleY = 0;
	bool beginGame = true;
	char keyEnter;
	int score = 0;
	int speed = 800;
	int speedBeforeKeyPress = speed;
	int increasedSpeed = 50;
	int minSpeed = 50;
	char lastKey = '\0';
	bool badAppleGenerated = false;
	int badAppleCounter = 0;

	//initialize apple
	getNextAppleCoordinates(appleX, appleY, offsetX, offsetY);

	std::cout << "Speed: " << GetSpeed();

	while (beginGame)
	{
		//display splash screen
		splashScreen();
		//wait for user input to continue
		keyEnter = _getch();
		if (keyEnter == ' ' || keyEnter == '\r')
		{
			//clear screen
			system("cls");
			//Collect player Initials
			playerInitials = collectUserName();
			//clear screen again
			system("cls");

			// Print game rules
			printGameRules();

			// Wait for the user to press Enter or space bar to continue
			while (true)
			{
				char key = _getch();
				if (key == ' ' || key == '\r') {
					break;  // Exit loop and start the game when space or Enter is pressed
				}
			}
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
		char currentKey = input();
		if (!gamePause)
		{
			//This removes the Pause Message
			setCursorPosition(offsetX, height + offsetY + 1);
			print("                                                   ");
			// Call adjustSpeedForKeyHold to manage the speed adjustment logic
			speed = adjustSpeedForKeyHold(speed, minSpeed, increasedSpeed, speedBeforeKeyPress);

			// Update the lastKey to the current key
			lastKey = currentKey;
			logic(appleX, appleY, snake, score, playerInitials, speed, minSpeed, badAppleX, badAppleY, badAppleGenerated, badAppleCounter);
			draw(appleX, appleY, snake, offsetX, offsetY, badAppleX, badAppleY);
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
	// Wait for the player to press Enter or Spacebar
	while (true)
	{
		char key = _getch(); // Get key input
		if (key == ' ' || key == '\r') // Check if it's Space (' ') or Enter ('\r')
		{
			break; 
		}
	}
	highScoreDisplay(playerInitials, score);
	return 0;
}

