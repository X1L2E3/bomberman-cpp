/*Added Player functionality*/

#include <iostream>
#include <conio.h>
using namespace std;

char grid[11][21];
int playerRow = 1, playerCol = 1;
string PlayerDir = "none";

void Stage()
{
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			if (i == 0 || j == 0 || j == 20 || i == 10 || i % 2 == 0 && j % 2 == 0)
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
void Draw()
{
	system("cls");
	// For debugging
	cout << "Player Row: " << playerRow << "\tPlayer Column: " << playerCol << endl;

	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			cout << grid[i][j];
		}
		cout << endl;
	}
}
void PlayerMovement()
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
void Input()
{
	if (_kbhit())
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
		}
		PlayerMovement();
	}
}

int main()
{
	Stage();
	Draw();
	while (true)
	{
		Input();
	}
}