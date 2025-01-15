#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <fstream>
using namespace std;

bool gameOver = false, stageOver = false, stageWin = false, exitGame = false, menu = true, gamePlayed = true;
const int ROWS = 11, COLS = 21;
const DWORD enemyInterval = 1000, bombInterval = 3000;
char grid[ROWS][COLS];
int playerRow = 1, playerCol = 1, playerScore = 0, playerLives = 3, playerBombs = 1, playerBombsPlaced = 0, playerBombLevel = 2, playerKills = 0;
int levelCount = 1, enemyPos[4][COLS], obstacleCount = 20, powerupCount = 5, enemyCount = 3;
int topScores[10] = { 0 };
int gateX = -1, gateY = -1;
DWORD bombTime[ROWS][COLS], stageTime, moveTime, enemyTime = GetTickCount();

void SaveTopScores(); void LoadTopScores(); void UpdateTopScores(); void DisplayTopScores();
void SetCursorPosition(short int x, short int y); void SetConsoleColor(int c); void ShowConsoleCursor(bool visible);
void Menu(); void Draw(); void Input(); void Game(); void PlayerScore(char type);
void StageSetObstacles(int obstacles); void StageSetEnemies(int enemies);
void StageSetPowerups(int powerups); void StageSetLevel(int level); void StageWin();
void StageSetup(); void StagePlay(int level, int obstacles, int powerups, int enemies, int bombs, int score);
void EnemyMovement(); void EnemyTimer();
void PlayerMovement(char playerDir);
void BombInteraction(char object); void BombExplosion(int bombRow, int bombCol); void BombTimer();
void Timeout(DWORD t = 15);

int main()
{
	Game();
	return 0;
}

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
	system("pause");
	system("cls");
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

void StageSetObstacles(int obstacles)
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
			if (((gateX == -1 && gateY == -1) && (rand() % obstacles == 0)) || obstacles == 1)
			{
				gateX = r;
				gateY = c;
			}
		}
	}
}
void StageSetEnemies(int enemies)
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
void StageSetPowerups(int powerups)
{
	while (powerups)
	{
		int r = 1 + rand() % ROWS-2, c = 1 + rand() % COLS-2;
		if (grid[r][c] == ' ' &&
			grid[r+1][c] != '+' && grid[r-1][c] != '+' &&
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
void StageSetLevel(int level)
{
	switch (level)
	{
		case 1:
		obstacleCount = 5, powerupCount = 0, enemyCount = 1;
		break;
		case 2:
		obstacleCount = 10, powerupCount = 0, enemyCount = 2;
		break;
		case 3:
		obstacleCount = 15, powerupCount = 1, enemyCount = 2;
		break;
		case 4:
		obstacleCount = 20, powerupCount = 2, enemyCount = 3;
		break;
		case 5:
		obstacleCount = 25, powerupCount = 2, enemyCount = 5;
		break;
	}
}
void StageSetup()
{
	system("cls");
	cout << "\n\n\t\tSTAGE " << levelCount;
	Timeout();

	gateX = gateY = -1;
	enemyTime = stageTime = GetTickCount();
	playerRow = playerCol = 1;
	stageOver = false;
	playerBombsPlaced = playerKills = 0;

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

	StageSetLevel(levelCount);
	StageSetObstacles(obstacleCount);
	StageSetPowerups(powerupCount);
	StageSetEnemies(enemyCount);

	grid[playerRow][playerCol] = 'P';

	system("cls");
	StagePlay(levelCount, obstacleCount, powerupCount, enemyCount, playerBombs, playerScore);
}
void StagePlay(int level, int obstacles, int powerups, int enemies, int bombs, int score)
{
	while (!stageOver)
	{
		Draw();
		Input();

		EnemyTimer();
		BombTimer();
	}

	if (stageWin)
	{
		cout << "\n\n\tYou win!\n";
		Timeout(10);

		if (level < 5)
			level++;
		else
			gameOver = true;
	}
	else if (gamePlayed == true)
	{
		if (playerLives > 0)
		{
			cout << "\n\n\tYou died...\n";
			Timeout(20);
			playerScore = score;
		}
		else
		{
			cout << "\n\n\tYou have lost all your lives!\n";
			Timeout(20);
			gameOver = true;
		}
	}

	obstacleCount = obstacles;
	enemyCount = enemies;
	powerupCount = powerups;
	levelCount = level;
	playerBombs = bombs;
}
void StageLose()
{
	stageOver = true;
	stageWin = false;
	if (playerLives > 0) playerLives--;
}
void StageWin()
{
	if (playerKills == enemyCount)
	{
		stageWin = true;
		stageOver = true;
	}
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
				StageLose();
			}
			else if (grid[newR][newC] == 'X')
			{
				grid[r][c] = ' ';
				playerScore += 5;
				playerKills++;
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

void BombInteraction(char type)
{
	switch (type)
	{
	case 'E':
		playerScore += 5;
		playerKills++;
		break;
	}
}
void BombExplosion(int bombRow, int bombCol)
{
	if (grid[bombRow][bombCol] == 'B' || grid[bombRow][bombCol] == 'P')
	{
		playerBombsPlaced--;
		bool forced = false;
		
		if (grid[bombRow][bombCol] == 'P') StageLose();
		grid[bombRow][bombCol] = 'X';
		for (int dir = 0; dir < 4; dir++)
		{
			for (int r = 1; r <= playerBombLevel; r++)
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
						StageLose();
					}
					else if (grid[newRow][newCol] == ' ' || grid[newRow][newCol] == '+'|| grid[newRow][newCol] == 'E')
					{
						BombInteraction(grid[newRow][newCol]);
						grid[newRow][newCol] = 'X';
					}
					else if (grid[newRow][newCol] == 'O')
					{
						BombInteraction(grid[newRow][newCol]);
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
			for (int r = 1; r <= playerBombLevel; r++)
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

void PlayerInteraction(char object)
{
	switch (object)
	{
	case '+':
		playerBombs++;
		playerScore += 1;
		break;
	case 'X':
	case 'E':
		StageLose();
		break;
	}
}
void PlayerMovement(char playerDir)
{
	if (GetTickCount() - moveTime < 250) return;
	moveTime = GetTickCount();

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
		grid[newRow][newCol] = 'P';
		playerRow = newRow;
		playerCol = newCol;
	}
	else if (grid[newRow][newCol] == 'E' || grid[newRow][newCol] == '+' || grid[newRow][newCol] == 'X')
	{
		PlayerInteraction(grid[newRow][newCol]);
		grid[newRow][newCol] = 'P';
		playerRow = newRow;
		playerCol = newCol;
	}
	else
	{
		grid[playerRow][playerCol] = 'P';
	}
	if (playerRow == gateX && playerCol == gateY) StageWin();
}

void Input()
{
	char playerDir = 'n';

	if (_kbhit())
	{
		char key = _getch();

		switch (key)
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
			if (!bombTime[playerRow][playerCol] && (playerBombsPlaced < playerBombs))
			{
				bombTime[playerRow][playerCol] = GetTickCount();
				playerBombsPlaced++;
				grid[playerRow][playerCol] = 'B';
			}
			break;
		case 27:
		case 8:
			stageOver = true;
			gameOver = true;
			menu = true;
			gamePlayed = false;
			break;
		}
		if (playerDir != 'n')
		{
			PlayerMovement(playerDir);
		}
	}
}
void Draw()
{
	SetCursorPosition(0, 0);
	SetConsoleColor(5);
	
	cout << "[DEBUG] Enemies: " << enemyCount << "\tKills: " << playerKills << endl;
	cout << "Player: " << playerRow << " " << playerCol << "\tGate: " << gateX << " " << gateY << endl;
	cout << "Score: " << playerScore << "\tLives: " << playerLives << endl << endl;

	for (int i = 0; i <= ROWS-1; i++)
	{
		cout << "\t";
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
				if (grid[i][j] == 'P')
				{
					SetConsoleColor(167);
					if (GetTickCount() - moveTime < 250) cout << ' ';
					else cout << "+";
				}
				else if (grid[i][j] == '!')
				{
					SetConsoleColor(5);
					cout << '!';
				}
			}
			else if (grid[i][j] == ' ' && (gateX == i && gateY == j))
			{
				SetConsoleColor(5);
				cout << 'I';
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
				case ' ':
				default:
					SetConsoleColor(0);
					cout << ' ';
				}
			}
		}
		cout << endl;
	}

	SetConsoleColor(12);
}
void Menu()
{
	int sel = 1, a, b, c, d;
	char key;

	while (menu)
	{
		SetCursorPosition(0, 0);
		SetConsoleColor(192);
		cout << "\t======================\n";
		cout << "\t=  BOMBER-MAN CLONE  =\n";
		cout << "\t======================\n\n";
		if (sel == 1)
		{
			a = 240;
			b = c = d = 7;
		}
		else if (sel == 2)
		{
			a = c = d = 7;
			b = 240;
		}
		else if (sel == 3)
		{
			a = b = d = 7;
			c = 240;
		}
		else if (sel == 4)
		{
			a = b = c = 7;
			d = 240;
		}

		SetConsoleColor(a);
		if (sel == 1 && (GetTickCount() % 1000 < 500)) cout << "\t> Play";
		else cout << "\t  Play";
		SetConsoleColor(b);
		if (sel == 2 && (GetTickCount() % 1000 < 500)) cout << "\t\t> Help";
		else cout << "\t\t  Help";
		SetConsoleColor(c);
		if (sel == 3 && (GetTickCount() % 1000 < 500)) cout << "\n\t> Scores";
		else cout << "\n\t  Scores";
		SetConsoleColor(d);
		if (sel == 4 && (GetTickCount() % 1000 < 500)) cout << "\t> Exit";
		else cout << "\t  Exit";

		SetConsoleColor(6);
		cout << "\n\n\t[Arrow keys]\tMove\n\t[Enter]\t\tSelect\n\t[Esc]\t\tQuit\n";
		SetConsoleColor(5);

		if (_kbhit()) key = _getch();
		switch (key)
		{
		case 'a':
		case 75:
			if (sel > 1)
				sel--;
			break;
		case 'd':
		case 77:
			if (sel < 4)
				sel++;
			break;
		case 'w':
		case 72:
			if (sel > 2)
				sel -= 2;
			break;
		case 's':
		case 80:
			if (sel < 3)
				sel += 2;
			break;
		case 'e':
		case 13:
			switch (sel)
			{
			case 1:
				gameOver = false;
				stageOver = false;
				exitGame = false;
				menu = false;
				break;
			case 2:
				//Help();
				system("pause");
				break;
			case 3:
				system("cls");
				DisplayTopScores();
				break;
			case 4:
				gameOver = true;
				exitGame = true;
				menu = false;
				gamePlayed = false;
				break;
			}
			system("cls");
			break;
		case 27:
			gameOver = true;
			exitGame = true;
			menu = false;
			gamePlayed = false;
			break;
		}
		key = '\0';
	}
}
void Game()
{
	ShowConsoleCursor(false);
	srand(time(0));

	while (!exitGame)
	{
		system("cls");
		LoadTopScores();
		Menu();

		while (!gameOver)
		{
			StageSetup();
			UpdateTopScores();
		}
		SaveTopScores();
		
		if (gamePlayed)
		{
			cout << "\nGame Over!\n\n";
			DisplayTopScores();
		}
	}
}

void Timeout(DWORD t)
{
    DWORD timeout = GetTickCount();
	while (GetTickCount() - timeout < (t*100));
    return;
}