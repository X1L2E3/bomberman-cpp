/*Added first game over condition
Known issues:
- One bomb's explosion clears another bomb's explosion
- Bomb explosion going through walls
- Player phases through Bomb
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

bool gameOver = false;
const int WIDTH = 31, HEIGHT = 11;
char grid[HEIGHT][WIDTH];
bool bombGrid[HEIGHT][WIDTH];
int playerRow = 1, playerCol = 1, bombCount = 3, bombLevel = 2;
string playerDir = "none";

void Stage()
{
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			if (i == 0 || i == 10)
			{
				grid[i][j] = '-';
			}
			else if (j == 0 || j == 20)
			{
				grid[i][j] = '|';
			}
			else if (i % 2 == 0 && j % 2 == 0)
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
	// Takes cursor to 0, 0 position, which then overwrites the existing text in console
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

	// For debugging
	cout << "Player Row: " << playerRow << "\tPlayer Column: " << playerCol << endl;
	cout << "Bombs: " << bombCount << endl;

	for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 20; j++)
		{
			if (bombGrid[i][j])
			{
				grid[i][j] = 'B';
				cout << grid[i][j];
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
		else if (grid[playerRow - 1][playerCol] == 'X')
			gameOver = true;
	}
	else if (playerDir == "down")
	{
		if (grid[playerRow + 1][playerCol] == ' ')
			playerRow++;
		else if (grid[playerRow + 1][playerCol] == 'X')
			gameOver = true;
	}
	else if (playerDir == "right")
	{
		if (grid[playerRow][playerCol + 1] == ' ')
			playerCol++;
		else if (grid[playerRow][playerCol + 1] == 'X')
			gameOver = true;
	}
	else if (playerDir == "left")
	{
		if (grid[playerRow][playerCol - 1] == ' ')
			playerCol--;
		else if (grid[playerRow][playerCol - 1] == 'X')
			gameOver = true;
	}

	grid[playerRow][playerCol] = 'P';
}
void Bomb()
{
	bombCount--;
	int bombRow = playerRow;
	int bombCol = playerCol;
	bombGrid[bombRow][bombCol] = true;

	this_thread::sleep_for(chrono::seconds(3));
	bombGrid[bombRow][bombCol] = false;
	bombCount++;
	
	if (grid[bombRow][bombCol] == 'P') gameOver = true;
	grid[bombRow][bombCol] = 'X';
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow - i][bombCol] == ' ')
			grid[bombRow - i][bombCol] = 'X';
		else if (grid[bombRow - i][bombCol] == 'P')
			gameOver = true;
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow + i][bombCol] == ' ')
			grid[bombRow + i][bombCol] = 'X';
		else if (grid[bombRow + i][bombCol] == 'P')
			gameOver = true;
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow][bombCol + i] == ' ')
			grid[bombRow][bombCol + i] = 'X';
		else if (grid[bombRow][bombCol + i] == 'P')
			gameOver = true;
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow][bombCol - i] == ' ')
			grid[bombRow][bombCol - i] = 'X';
		else if (grid[bombRow][bombCol - i] == 'P')
			gameOver = true;

	this_thread::sleep_for(chrono::seconds(1));
	grid[bombRow][bombCol] = ' ';
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow - i][bombCol] == 'X')
			grid[bombRow - i][bombCol] = ' ';
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow + i][bombCol] == 'X')
			grid[bombRow + i][bombCol] = ' ';
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow][bombCol + i] == 'X')
			grid[bombRow][bombCol + i] = ' ';
	for (int i = 1; i <= bombLevel; i++)
		if (grid[bombRow][bombCol - i] == 'X')
			grid[bombRow][bombCol - i] = ' ';
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
	cout << "\nGame Over!" << endl;
}