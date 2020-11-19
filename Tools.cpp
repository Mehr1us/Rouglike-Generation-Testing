#include "main.h"
#include <random>
#include <time.h>
#include <math.h>

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

bool Tools::touchingBorder(int x, int y)
{
	if (x == 0 || x == 48)return true;
	if (y == 0 || y == 18)return true;
	return false;
}

void Tools::render(int map[][49], int a, int b)
{
	char symbmap[] = { '.', '#', '!' };
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			printf("%c", symbmap[map[i][j]]);
		}
		printf("\n");
	}
}

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
