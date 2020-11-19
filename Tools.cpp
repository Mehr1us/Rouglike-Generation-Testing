#include "main.h"
#include <random>
#include <time.h>
#include <math.h>

//scrambles my 4 int array based on the "seed"
void Tools::scrambler(int arr[], int l, int c, int loop, char seed[40])
{
	srand((int)seed[c] * (int)seed[c + 1] * 3737 * (loop + 1));
	int j = rand() % l;
	int jr[20] = { j, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int tempArr[20];
	bool scrambled = false;
	for (int i = 0; i < l; i++)
	{
		tempArr[i] = arr[i];
	}
	for (int i = 0; i < l; i++)
	{
		arr[i] = tempArr[j];
		int d = 0;
		do
		{
			int c = 0;
			j = rand() % l;
			if (i + 1 < l)
			{
				for (int k = 0; k < l; k++)
				{
					if (jr[k] != j)
					{
						c++;
					}
				}
				if (c == l)
				{
					jr[i + 1] = j;
					break;
				}
			}
			else break;
			d++;
		} while (true);
	}
}

//Checks how many of int value are in the map
int Tools::valFinder(int map[19][49], int value)
{
	int a = 19; int b = 49; int x = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (map[i][j] == value)x++;
		}
	}
	return x;
}

//checks if the current x and y coords are on the border 
bool Tools::touchingBorder(int x, int y)
{
	if (x == 0 || x == 48)return true;
	if (y == 0 || y == 18)return true;
	return false;
}

//draws the map
void Tools::render(int map[][49], int a, int b)
{
	//'.' represent floors, '#' represent walls and '!' represents the current x, y pos only seen if debug or showGen = true;
	char symbmap[] = { ' ', '#', '!' ,205,186, 'x', '.',201,187,200,188};

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			printf("%c", symbmap[map[i][j]]);
		}
		printf("\n");
	}
}

//count num of neighbours that are equal to int tileno
int Tools::nebCount(int map[][49], int i, int j, int tileno)
{
	int neb = 0;
	// i = columns, j = rows
	if (map[i - 1][j - 1] == tileno)neb++;
	if (map[i - 1][j] == tileno)neb++;
	if (map[i - 1][j + 1] == tileno)neb++;
	if (map[i][j - 1] == tileno)neb++;
	if (map[i][j + 1] == tileno)neb++;
	if (map[i + 1][j - 1] == tileno)neb++;
	if (map[i + 1][j] == tileno)neb++;
	if (map[i + 1][j + 1] == tileno)neb++;

	return neb;
}

//checks if the 4 neighbours are all floors // technically no longer needed
bool Tools::noDestination(int map[][49], int i, int j)
{
	int neb = 0;
	if (map[i + 1][j] == 0 || i + 1 == 48)neb++;
	if (map[i - 1][j] == 0 || i - 1 == 0)neb++;
	if (map[i][j + 1] == 0 || j + 1 == 18)neb++;
	if (map[i][j - 1] == 0 || j - 1 == 0)neb++;
	if (neb == 4)return true;
	return false;
}

//compares two 1d int arrays (unused so far)
bool Tools::arrcmp(int a[], int b[])
{
	size_t n = sizeof(a) / sizeof(a[0]);
	size_t m = sizeof(b) / sizeof(b[0]);
	int l = 0;
	if (n != m)return false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (a[i] == b[j])l++;
		}
	}
	if (l == n)return true;
	return false;
}
