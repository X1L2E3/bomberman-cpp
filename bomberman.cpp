/*Added game loop
Notes:
- Basic structure of game is complete
- Flickering is due to 'system("cls")'. Needs a fix
- Added additional comments to the code
*/

#include <iostream>
#include <conio.h>		// _getch()
#include <windows.h>	// Used for Sleep() function (Not used here)
using namespace std;

bool gameOver = false;
char grid[11][21];
int playerRow = 1, playerCol = 1;
string PlayerDir = "none";	// Should be replaced wih 'char'

void Stage()	// Initialize the array with our map
{
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			if (i == 0 || j == 0 || j == 20 || i == 10 || (i % 2 == 0 && j % 2 == 0))
			{
				grid[i][j] = '#';
			}
			else
			{
				grid[i][j] = ' ';
			}
		}
	}
	grid[playerRow][playerCol] = 'P';
}
void Draw()	// Draw the array
{
	system("cls");
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			cout << grid[i][j];
		}
		cout << endl;
	}
}
void PlayerMovement()	// Move the player
{
	int tempRow = playerRow;
	int tempCol = playerCol;

	if (PlayerDir == "up")
	{
		playerRow--;
	}
	else if (PlayerDir == "down")
	{
		playerRow++;
	}
	else if (PlayerDir == "right")
	{
		playerCol++;
	}
	else if (PlayerDir == "left")
	{
		playerCol--;
	}
	PlayerDir = "none";

	grid[tempRow][tempCol] = ' ';
	grid[playerRow][playerCol] = 'P';
}
void Input()	// Take input from user
{
	if (_kbhit())	// Asynchronous input
	{
		switch (_getch())
		{
		case 'w':
		case 72:
			if (grid[playerRow - 1][playerCol] == ' ')
				PlayerDir = "up";
			break;
		case 's':
		case 80:
			if (grid[playerRow + 1][playerCol] == ' ')
				PlayerDir = "down";
			break;
		case 'd':
		case 77:
			if (grid[playerRow][playerCol + 1] == ' ')
				PlayerDir = "right";
			break;
		case 'a':
		case 75:
			if (grid[playerRow][playerCol - 1] == ' ')
				PlayerDir = "left";
			break;
		case 'x':
			gameOver = true;
			break;
		}
		PlayerMovement();
	}
}

int main()
{
	Stage();
	while (!gameOver)	// Game loop
	{
		Draw();
		Input();
	}
	cout << "Game Over!" << endl;
}