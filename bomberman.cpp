#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

bool gameOver = false;
const int ROWS = 11, COLS = 41;
char grid[ROWS][COLS];
int playerRow = 1, playerCol = 1, playerScore = 0;
int bombCount = 3, bombLevel = 2, obstacleCount = 20, powerupCount = 5;
string playerDir = "none";
time_t bombTime[ROWS][COLS];

void SetCursorPosition(short int x, short int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD CursorPos = {x, y};
	SetConsoleCursorPosition(hConsole, CursorPos);
}
void SetConsoleColor(int c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, c);
}

void Menu()
{
	cout << "Bomber man" << endl;
	cout << "Press b to place bomb, w, a, s, d to move" << endl;
	cout << "Press x to quit" << endl;
	cout << "[ANY KEY] Play game" << endl;
	_getch();
	system("cls");
}
void StageObstacles(int obstacles)
{
	while (obstacles)
	{
		int r = 1 + rand() % ROWS-2, c = 1 + rand() % COLS-2;
		if (grid[r][c] == ' ' &&
			r != playerRow + 1 && c != playerCol && 
			r != playerRow + 2 && c != playerCol &&
			r != playerRow && c != playerCol + 1 &&
			r != playerRow && c != playerCol + 2)
		{
			grid[r][c] = 'O';
			obstacles--;
		}
	}
}
void Stage()
{
	for (int r = 0; r <= ROWS-1; r++)
	{
		for (int c = 0; c <= COLS-1; c++)
		{
			if (r == 0 || r == ROWS-1)
			{
				grid[r][c] = '-';
			}
			else if (c == 0 || c == COLS-1)
			{
				grid[r][c] = '|';
			}
			else if (r % 2 == 0 && c % 2 == 0)
			{
				grid[r][c] = '#';
			}
			else
			{
				grid[r][c] = ' ';
			}
		}
	}

	for (int i = 0; i <= ROWS; i++)
	{
		for (int j = 0; j <= COLS; j++)
		{
			bombTime[i][j] = 0;
		}
	}

	StageObstacles(obstacleCount);
}

void Draw()
{
	SetCursorPosition(0, 0);

	// For debugging
	cout << "Player Row: " << playerRow << "\tPlayer Column: " << playerCol << endl;
	cout << "Score: " << playerScore << "\tBombs: " << bombCount << endl;

	for (int i = 0; i <= ROWS-1; i++)
	{
		for (int j = 0; j <= COLS-1; j++)
		{
			if (bombTime[i][j])
			{
				SetConsoleColor(4);
			}
			else if (playerRow == i && playerCol == j)
			{
				SetConsoleColor(5);
				grid[i][j] = 'P';
			}
			else
			{
				switch (grid[i][j])
				{
				case '-':
				case '|':
					SetConsoleColor(2);
					break;
				case 'X':
					SetConsoleColor(4);
					break;
				case '#':
					SetConsoleColor(7);
					break;
				case 'O':
					SetConsoleColor(6);
					break;
				default:
					SetConsoleColor(8);
				}
			}
			cout << grid[i][j];
		}
		cout << endl;
	}
}
void PlayerMovement(char playerDir)
{
	if (playerDir != 'n')
	{
		int newRow = playerRow;
		int newCol = playerCol;

		if (playerDir == 'u')
			newRow--;
		else if (playerDir == 'd')
			newRow++;
		else if (playerDir == 'r')
			newCol++;
		else if (playerDir == 'l')
			newCol--;

		if (grid[playerRow][playerCol] == 'P')
			grid[playerRow][playerCol] = ' ';

		if (grid[newRow][newCol] == ' ')
		{
			playerRow = newRow;
			playerCol = newCol;
		}
		else if (grid[newRow][newCol] == 'X')
		{
			gameOver = true;
		}
	}
}
void Score(char type)
{
	switch (type)
	{
	case 'O':
		playerScore += 10;
		break;
	case 'E':
		playerScore += 50;
	}

}
void Bomb(int bombRow, int bombCol)
{
	bombCount++;

	bool forced = false;
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

			if (newRow > 0 && newRow < ROWS && newCol > 0 && newCol < COLS)
			{
				if (newRow == playerRow && newCol == playerCol)
				{
					gameOver = true;
				}
				else if (grid[newRow][newCol] == ' ' || grid[newRow][newCol] == '+')
				{
					grid[newRow][newCol] = 'X';
				}
				else if (grid[newRow][newCol] == 'O' || grid[newRow][newCol] == 'E')
				{
					Score(grid[newRow][newCol]);
					grid[newRow][newCol] = 'X';
					break;
				}
				else if (grid[newRow][newCol] == '#')
				{
					break;
				}
				else if (grid[newRow][newCol] == 'B')
				{
					forced = true;
					Bomb(newRow, newCol);
					bombTime[newRow][newCol] = 0;
					grid[newRow][newCol] = 'X';
				}
			}
		}
	}

	if (!forced)
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

			if (newRow > 0 && newRow < ROWS && newCol > 0 && newCol < COLS)
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
	for (int r = 0; r <= ROWS; r++)
	{
		for (int c = 0; c <= COLS; c++)
		{
			if (bombTime[r][c] > 0)
			{
				if (time(0) - bombTime[r][c] >= 3)
				{
					thread bombThread(Bomb, r, c);
					bombThread.detach();
					bombTime[r][c] = 0;
				}
			}
		}
	}
}
void Input()
{
	char playerDir = 'n';

	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
		case 72:
			playerDir = 'u';
			break;
		case 's':
		case 80:
			playerDir = 'd';
			break;
		case 'd':
		case 77:
			playerDir = 'r';
			break;
		case 'a':
		case 75:
			playerDir = 'l';
			break;
		case 'b':
		case ' ':
			if (!bombTime[playerRow][playerCol] && bombCount)
			{
				bombTime[playerRow][playerCol] = time(0);
				bombCount--;
				grid[playerRow][playerCol] = 'B';
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
	Menu();
	Stage();
	
	while (!gameOver)
	{
		Draw();
		Input();
		
		BombTimer();
	}
	cout << "\nGame Over!" << endl;
}