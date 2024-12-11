/*Added multiple bombs functionality*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

bool gameOver = false;
const int WIDTH = 21, HEIGHT = 11;
char grid[HEIGHT][WIDTH];
bool bombGrid[HEIGHT][WIDTH];
int playerRow = 1, playerCol = 1, bombCount = 3;
string playerDir = "none";
string activity = "";

void Stage()
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
void Draw()
{
	system("cls");
	// For debugging
	cout << "Player Row: " << playerRow << "\tPlayer Column: " << playerCol << endl;
	cout << "Bombs: " << bombCount << endl;
	cout << "Last activity: " << activity << endl << endl;

	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			if (bombGrid[i][j])
			{
				cout << "B";
			}
			else
			{
				cout << grid[i][j];
			}
		}
		cout << endl;
	}
}
void PlayerMovement(string playerDir)
{
	if (grid[playerRow][playerCol] == 'P')
		grid[playerRow][playerCol] = ' ';

	if (playerDir == "up")
	{
		if (grid[playerRow - 1][playerCol] == ' ')
			playerRow--;
	}
	else if (playerDir == "down")
	{
		if (grid[playerRow + 1][playerCol] == ' ')
			playerRow++;
	}
	else if (playerDir == "right")
	{
		if (grid[playerRow][playerCol + 1] == ' ')
			playerCol++;
	}
	else if (playerDir == "left")
	{
		if (grid[playerRow][playerCol - 1] == ' ')
			playerCol--;
	}

	grid[playerRow][playerCol] = 'P';
}
void Bomb()
{
	activity = "Placed Bomb at " + to_string(playerRow) + ", " + to_string(playerCol);

	bombCount--;
	int bombRow = playerRow;
	int bombCol = playerCol;
	bombGrid[bombRow][bombCol] = true;

	this_thread::sleep_for(chrono::seconds(3));
	bombGrid[bombRow][bombCol] = false;
	bombCount++;
	grid[bombRow][bombCol] = 'X';
	if (grid[bombRow - 1][bombCol] == ' ') { grid[bombRow - 1][bombCol] = 'X'; }
	if (grid[bombRow + 1][bombCol] == ' ') { grid[bombRow + 1][bombCol] = 'X'; }
	if (grid[bombRow][bombCol + 1] == ' ') { grid[bombRow][bombCol + 1] = 'X'; }
	if (grid[bombRow][bombCol - 1] == ' ') { grid[bombRow][bombCol - 1] = 'X'; }

	this_thread::sleep_for(chrono::seconds(1));
	grid[bombRow][bombCol] = ' ';
	if (grid[bombRow - 1][bombCol] == 'X') { grid[bombRow - 1][bombCol] = ' '; }
	if (grid[bombRow + 1][bombCol] == 'X') { grid[bombRow + 1][bombCol] = ' '; }
	if (grid[bombRow][bombCol + 1] == 'X') { grid[bombRow][bombCol + 1] = ' '; }
	if (grid[bombRow][bombCol - 1] == 'X') { grid[bombRow][bombCol - 1] = ' '; }
}
void Input()
{
	string playerDir = "none";

	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
		case 72:
			playerDir = "up";
			break;
		case 's':
		case 80:
			playerDir = "down";
			break;
		case 'd':
		case 77:
			playerDir = "right";
			break;
		case 'a':
		case 75:
			playerDir = "left";
			break;
		case 'b':
		case ' ':
		if (!bombGrid[playerRow][playerCol] && bombCount)
		{
			thread bombThread(Bomb);
			bombThread.detach();

			bombGrid[playerRow][playerCol] = true;
		}
			break;
		case 'x':
			gameOver = true;
			break;
		}
		PlayerMovement(playerDir);
	}
}

int main()
{
	Stage();
	while (!gameOver)
	{
		Draw();
		Input();
	}
	cout << "Game Over!" << endl;
}