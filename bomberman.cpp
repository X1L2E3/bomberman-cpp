#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <fstream>
using namespace std;

bool gameOver = false;
const int ROWS = 11, COLS = 41;
const DWORD enemyInterval = 1000, bombInterval = 3000;
char grid[ROWS][COLS];
int playerRow = 1, playerCol = 1, playerScore = 0, topScores[10] = { 0 };
int enemyPos[4][COLS], bombCount = 1, bombLevel = 2, obstacleCount = 20, powerupCount = 5, enemyCount = 3;
time_t lastTime;
DWORD bombTime[ROWS][COLS], stageTime, enemyTime = GetTickCount();

void SaveTopScores()
{
	ofstream file("top_scores.txt");
	if (file.is_open())
	{
		for (int i = 0; i < 10; i++) {
			file << topScores[i] << endl;
		}
		file.close();
	}
}
void LoadTopScores()
{
	ifstream file("top_scores.txt");
	if (file.is_open())
	{
		int i = 0;
		while (file >> topScores[i] && i < 10) {
			i++;
		}
		file.close();
	}
}
void UpdateTopScores()
{
	for (int i = 0; i < 10; i++) {
		if (playerScore > topScores[i])
		{
			for (int j = 2; j > i; j--) {
				topScores[j] = topScores[j - 1];
			}
			topScores[i] = playerScore;
			break;
		}
	}
	SaveTopScores();
}
void DisplayTopScores()
{
	cout << "=== Top 10 Scores ===" << endl;
	for (int i = 0; i < 10; i++) {
		cout << i + 1 << ": " << topScores[i] << endl;
	}
	cout << "====================" << endl;
}

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
void ShowConsoleCursor(bool visible)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = visible; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void Menu()
{
	cout << "\n\tBomber man" << endl;
	cout << "\tPress b to place bomb, w, a, s, d to move" << endl;
	cout << "\tPress x to quit" << endl;
	cout << "\n\t[ANY KEY] Play game" << endl;
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
void StageEnemies(int enemies)
{
	int i = 0;
	while (enemies)
	{
		int r = rand() % ROWS - 1, c = rand() % COLS - 1, d = rand() % 4;
		if (grid[r][c] == ' ' &&
			r != playerRow + 1 && c != playerCol && 
			r != playerRow + 2 && c != playerCol &&
			r != playerRow && c != playerCol + 1 &&
			r != playerRow && c != playerCol + 2)
		{
			enemyPos[0][i] = r;		// Row
			enemyPos[1][i] = c;		// Column
			enemyPos[2][i] = d;		// Direction
			enemyPos[3][i] = 0;		// Move count
			grid[r][c] = 'E';
			enemies--;
			i++;
		}
	}
}
void StagePowerups(int powerups)
{
	while (powerups)
	{
		int r = 1 + rand() % ROWS-2, c = 1 + rand() % COLS-2;
		if (grid[r][c] == ' ' &&
			grid[r+1][c] != '+' &&grid[r-1][c] != '+' &&
			grid[r][c+1] != '+' && grid[r][c-1] != '+' &&
			r != playerRow + 1 && c != playerCol && 
			r != playerRow + 2 && c != playerCol &&
			r != playerRow && c != playerCol + 1 &&
			r != playerRow && c != playerCol + 2)
		{
			grid[r][c] = '+';
			powerups--;
		}
	}
}
void Stage()
{
	enemyTime = stageTime = GetTickCount();
	lastTime = time(0);

	for (int r = 0; r <= ROWS-1; r++)
	{
		for (int c = 0; c <= COLS-1; c++)
		{
			if (r == 0 || r == ROWS-1 || c == 0 || c == COLS-1 || (r % 2 == 0 && c % 2 == 0))
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
	StagePowerups(powerupCount);
	StageEnemies(enemyCount);

	grid[playerRow][playerCol] = 'P';
}
void Draw()
{
	SetCursorPosition(0, 0);
	SetConsoleColor(5);
	
	cout << "Score: " << playerScore << "\tTime: " << (time(0) - lastTime) << endl << endl;

	for (int i = 0; i <= ROWS-1; i++)
	{
		for (int j = 0; j <= COLS-1; j++)
		{
			if ((GetTickCount() - bombTime[i][j] >= 500 && GetTickCount() - bombTime[i][j] < 1000) ||
				(GetTickCount() - bombTime[i][j] >= 1500 && GetTickCount() - bombTime[i][j] < 2000) ||
				(GetTickCount() - bombTime[i][j] >= 2500 && GetTickCount() - bombTime[i][j] < 3000))
			{
				SetConsoleColor(127);
				cout << ' ';
			}
			else if (bombTime[i][j])
			{
				SetConsoleColor(197);
				cout << ' ';
			}
			else if (playerRow == i && playerCol == j)
			{
				SetConsoleColor(167);
				cout << ' ';
			}
			else
			{
				switch (grid[i][j])
				{
				case 'X':
					SetConsoleColor(68);
					cout << ' ';
					break;
				case '#':
					SetConsoleColor(136);
					cout << ' ';
					break;
				case 'O':
					SetConsoleColor(273);
					cout << ' ';
					break;
				case 'E':
					SetConsoleColor(85);
					cout << ' ';
					break;
				case '+':
					SetConsoleColor(270);
					cout << '+';
					break;
				default:
					SetConsoleColor(0);
					cout << grid[i][j];
				}
			}
		}
		cout << endl;
	}

	SetConsoleColor(12);
}

void EnemyMovement()
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (grid[enemyPos[0][i]][enemyPos[1][i]] == 'E')
		{
			int r = enemyPos[0][i];
			int c = enemyPos[1][i];
			int dir = enemyPos[2][i];
			int moves = enemyPos[3][i];

			int newR = r;
			int newC = c;

			if (dir == 0)
				newR--;
			else if (dir == 1)
				newR++;
			else if (dir == 2)
				newC--;
			else if (dir == 3)
				newC++;
			moves++;

			if (newR == playerRow && newC == playerCol)
			{
				gameOver = true;
			}
			else if (grid[newR][newC] == 'X')
			{
				grid[r][c] = ' ';
				break;
			}
			else if (grid[newR][newC] == ' ')
			{
				if (moves <= 4)
				{
					grid[r][c] = ' ';
					grid[newR][newC] = 'E';
					enemyPos[0][i] = newR;
					enemyPos[1][i] = newC;
					enemyPos[3][i] = moves;
				}
				else
				{
					enemyPos[2][i] = rand() % 4;
					enemyPos[3][i] = 0;
				}
			}
			else
			{
				enemyPos[2][i] = rand() % 4;
			}
		}
	}
}
void EnemyTimer()
{
	if (GetTickCount() - enemyTime >= enemyInterval)
	{
		EnemyMovement();
		enemyTime = GetTickCount();
	}
}
void PlayerInteraction(char object)
{
	switch (object)
	{
	case '+':
		bombCount++;
		playerScore += 20;
		break;
	case 'X':
	case 'E':
		gameOver = true;
	}
}
void PlayerMovement(char playerDir)
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
	else if (grid[newRow][newCol] == 'E' || grid[newRow][newCol] == '+' || grid[newRow][newCol] == 'X')
	{
		PlayerInteraction(grid[newRow][newCol]);
		playerRow = newRow;
		playerCol = newCol;
	}
	grid[playerRow][playerCol] = 'P';
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
void BombExplosion(int bombRow, int bombCol)
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
					grid[newRow][newCol] = '!';
					gameOver = true;
				}
				else if (grid[newRow][newCol] == ' ' || grid[newRow][newCol] == '+'|| grid[newRow][newCol] == 'E')
				{
					Score(grid[newRow][newCol]);
					grid[newRow][newCol] = 'X';
				}
				else if (grid[newRow][newCol] == 'O')
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
					if (bombTime[newRow][newCol] > 0)
					{
						forced = true;
						BombExplosion(newRow, newCol);
						bombTime[newRow][newCol] = 0;
						grid[newRow][newCol] = 'X';
					}
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
			if (bombTime[r][c] != 0)
			{
				if (GetTickCount() - bombTime[r][c] >= bombInterval)
				{
					thread bombThread(BombExplosion, r, c);
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
				bombTime[playerRow][playerCol] = GetTickCount();
				bombCount--;
				grid[playerRow][playerCol] = 'B';
			}
			break;
		case 27:
		case 8:
			gameOver = true;
			break;
		}
		if (playerDir != 'n') PlayerMovement(playerDir);
	}
}

int main()
{
	srand(time(0));
	ShowConsoleCursor(false);

	LoadTopScores();
	Menu();
	Stage();
	
	while (!gameOver)
	{
		Draw();
		Input();

		EnemyTimer();
		BombTimer();
	}
	UpdateTopScores();
	SaveTopScores();
	
	cout << "\nGame Over!\n\n";
	DisplayTopScores();
}