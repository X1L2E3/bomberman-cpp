#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

bool gameOver = false;
const int WIDTH = 41, HEIGHT = 11;
char grid[HEIGHT][WIDTH];
int playerRow = 1, playerCol = 1, bombCount = 3, bombLevel = 2, obstacleCount = 20;
string playerDir = "none";
time_t bombTime[HEIGHT][WIDTH];

void Stage()
{
	for (int i = 0; i <= HEIGHT-1; i++)
	{
		for (int j = 0; j <= WIDTH-1; j++)
		{
			if (i == 0 || i == HEIGHT-1)
			{
				grid[i][j] = '-';
			}
			else if (j == 0 || j == WIDTH-1)
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

	for (int i = 0; i <= HEIGHT; i++)
	{
		for (int j = 0; j <= WIDTH; j++)
		{
			bombTime[i][j] = 0;
		}
	}

	while (obstacleCount)
{
	int obstacleRow = 1 + rand() % HEIGHT-2, obstacleCol = 1 + rand() % WIDTH-2;
	if (grid[obstacleRow][obstacleCol] == ' ' &&
		obstacleRow != playerRow + 1 && obstacleCol != playerCol && 
		obstacleRow != playerRow - 1 && obstacleCol != playerCol &&
		obstacleRow != playerRow && obstacleCol != playerCol + 1 &&
		obstacleRow != playerRow && obstacleCol != playerCol - 1)
	{
		grid[obstacleRow][obstacleCol] = 'O';
		obstacleCount--;
	}
}
}
void Draw()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

	// For debugging
	cout << "Player Row: " << playerRow << "\tPlayer Column: " << playerCol << endl;
	cout << "Bombs: " << bombCount << endl;

	for (int i = 0; i <= HEIGHT-1; i++)
	{
		for (int j = 0; j <= WIDTH-1; j++)
		{
			if (bombTime[i][j])
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
void Bomb(int bombRow, int bombCol)
{
	bombCount++;
	if (grid[bombRow][bombCol] == 'P') gameOver = true;
	grid[bombRow][bombCol] = 'X';
	for (int dir = 0; dir < 4; dir++)
	{
		for (int r = 1; r <= bombLevel; r++)
		{
			int newRow = bombRow, newCol = bombCol;

			if (dir == 0) newRow = bombRow - r;
			else if (dir == 1) newRow = bombRow + r;
			else if (dir == 2) newCol = bombCol - r;
			else if (dir == 3) newCol = bombCol + r;

			if (newRow > 0 && newRow < HEIGHT && newCol > 0 && newCol < WIDTH)
			{
				if (newRow == playerRow && newCol == playerCol)
				{
					gameOver = true;
				}
				else if (grid[newRow][newCol] == ' ' || grid[newRow][newCol] == '+')
				{
					grid[newRow][newCol] = 'X';
				}
				else if (grid[newRow][newCol] == 'O')
				{
					grid[newRow][newCol] = 'X';
					break;
				}
				else if (grid[newRow][newCol] == '#')
				{
					break;
				}
				else if (grid[newRow][newCol] == 'B')
				{
					grid[newRow][newCol] = 'X';
				}
			}
		}
	}

	this_thread::sleep_for(chrono::seconds(1));

	if (grid[bombRow][bombCol] == 'X' || grid[bombRow][bombCol] == '!') grid[bombRow][bombCol] = ' ';

	for (int dir = 0; dir < 4; dir++)
	{
		for (int r = 1; r <= bombLevel; r++)
		{
			int newRow = bombRow, newCol = bombCol;

			if (dir == 0) newRow = bombRow - r;
			else if (dir == 1) newRow = bombRow + r;
			else if (dir == 2) newCol = bombCol - r;
			else if (dir == 3) newCol = bombCol + r;

			if (newRow > 0 && newRow < HEIGHT && newCol > 0 && newCol < WIDTH)
			{
				if (grid[newRow][newCol] == 'X' || grid[newRow][newCol] == '!')
				{
					grid[newRow][newCol] = ' ';
				}
			}
		}
	}
}
void BombTimer()
{
	for (int i = 0; i <= HEIGHT; i++)
	{
		for (int j = 0; j <= WIDTH; j++)
		{
			if (bombTime[i][j] > 0)
			{
				if (time(0) - bombTime[i][j] >= 3)
				{
					thread bombThread(Bomb, i, j);
					bombThread.detach();
					bombTime[i][j] = 0;
				}
			}
		}
	}
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
			if (!bombTime[playerRow][playerCol] && bombCount)
			{
				bombTime[playerRow][playerCol] = time(0);
				bombCount--;
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
	cout << "Bomberman" << endl;
	cout << "Press b to place bomb, w, a, s, d to move" << endl;
	cout << "Press x to quit" << endl;
	cout << "[ANY KEY] Play game" << endl;
	_getch();
	system("cls");

	while (!gameOver)
	{
		Draw();
		Input();
		
		BombTimer();
	}
	cout << "\nGame Over!" << endl;
}