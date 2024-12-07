///*Printing the Grid*/
//
//#include <iostream>
//using namespace std;
//
//char grid[11][21];
//
//int main()
//{
//	for (int i = 0; i <= 10; i++)
//	{
//		for (int j = 0; j <= 20; j++)
//		{
//			if (i == 0 || j == 0 || j == 20 || i == 10 || i % 2 == 0 && j % 2 == 0)
//			{
//				grid[i][j] = '#';
//			}
//			else
//			{
//				grid[i][j] = ' ';
//			}
//		}
//	}
//
//	for (int i = 0; i <= 10; i++)
//	{
//		for (int j = 0; j <= 20; j++)
//		{
//			
//			cout << grid[i][j];
//		}
//		cout << endl;
//	}
//}