#include "main.h"

bool pathgen = 1;

Point::Point(int x, int y, int id, int roomSize) : x(x), y(y), id(id), roomSize(roomSize) {}

Path::Path() {}

void PathTest::getSeed(char seed[40])
{
	int len = (int)strlen(seed);
	char input[40];
	printf("Enter a seed (0 for random): ");
	scanf_s("%s", input, 40);
	if (!strcmp(input, "0\0"))
	{
		if (debug)printf("rand seed\n");
		srand((unsigned int)time(NULL));
		len = (rand() % 36) + 4;
		for (int i = 0; i < len; i++) seed[i] = rand() % 128;
	}
	else if (strlen(input) >= 3)
	{
		for (int i = 0; i < strlen(input); i++)
		{
			seed[i] = input[i];
		}
	}
	if (seed[0] == seed[1])
	{
		seed[1] *= 10;
		seed[1] %= 128;
	}
	if (debug)
	{
		for (int i = 0; i < strlen(seed); i++)
		{
			printf("%c", seed[i]);
		}
		printf("<%d>\n", (int)strlen(input));
		system("pause");
		system("cls");
	}
}

unsigned char map[40][150]; //6000 tiles


void PathTest::render()
{
	char chMap[] = { '.', 219, 'O','*' };
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 150; x++)
		{
			printf("%c", chMap[map[y][x]]);
		}
		printf("\n");
	}
}

void PathTest::makePath(int seed, int x1, int y1, int x2, int y2)
{
	int xa, ya, xb, yb, xr, yr, j = 0; double xlen, ylen, xlenr, ylenr;
	if (x2 > x1) { xa = x2, ya = y2, xb = x1, yb = y1; }
	else { xa = x1, ya = y1, xb = x2, yb = y2; }
	int x = xb, y = yb;
	xr = xa; yr = ya;
	if (!pathgen)
	{
		int xlen = xa - xb;
		int ylen = ya - yb;
		bool flip = false;
		if (ylen < 0)flip = true;
		for (int i = xb; i <= xa; i++)
		{
			map[yb][i] = 3;
			j = i;
		}
		switch (flip) {
		case 0:
			for (int i = yb; i <= ya; i++)
			{
				map[i][j] = 3;
			}
			break;
		case 1:
			for (int i = yb; i >= ya; i--)
			{
				map[i][j] = 3;
			}
			break;
		}
	}
	else
	{
		bool flip = false;
		if (ya < yb)flip = true;
		xlenr = abs((double)xa - (double)xb) + 1;
		ylenr = abs((double)ya - (double)yb) + 1;
		srand(seed);
		do {
			xlen = (abs((double)xa - (double)xb) + 1) / xlenr;
			ylen = (abs((double)ya - (double)yb) + 1) / ylenr;
			if (xlen == ylen)
			{
				bool randDir = rand() % 2;
				switch (randDir)
				{
				case 0:
					if (x != xa)x++;
					break;
				case 1:
					if (y != ya)y = (!flip) ? y + 1 : y - 1;
					break;
				}
			}
			else {
				if (xlen > ylen)x++;
				else y = (!flip) ? y + 1 : y - 1;
			}
			if (x > 0 && x < 140 && y > 0 && y < 40)map[y][x] = 3;
			if (x == xa && y == ya)
			{
				if (debug)printf("(%d,%d)\n", x, y);
				break;
			}
		} while (true);
	}
	return;
}

void PathTest::makeRoom(int x, int y, int roomSize)
{
	for (int y1 = y - roomSize; y1 <= y + roomSize; y1++)
	{
		for (int x1 = x - roomSize; x1 <= x + roomSize; x1++)
		{
			if (map[y1][x1] != 3)map[y1][x1] = 0;
		}
	}
	if (debug)printf("%d (%d, %d)\n", roomSize, x, y);
	return;
}

void PathTest::generate()
{
	system("cls");
	Path path;
	//generate
	char seed[40];
	for (int i = 0; i < 40; i++)seed[i] = 0;
	getSeed(seed);
	char input[20];
	for (int i = 0; i < 20; i++)input[i] = 0;
	do 
	{
		//
		printf("Linear or non-linear paths?\n");
		scanf_s("%s", input, sizeof(input));
		if (!strcmp(input, "linear") || !strcmp(input, "l") || !strcmp(input, "0")) { pathgen = 0; break; }
		else if (!strcmp(input, "non-linear") || !strcmp(input, "nl") || !strcmp(input, "1")) { pathgen = 1; break; }
		else printf("Only accepted input (in lowercase) is 'linear' ('l', '0') or 'non-linear' ('nl', '1')\n");
	} while (true);

	for (int y = 0; y < 40; y++) //40 rows
	{
		for (int x = 0; x < 150; x++) //150 columns
		{
			map[y][x] = 1;
		}
	}

	int seedi = 1;
	for (int i = 0; i < strlen(seed); i++)
	{
		seedi *= (seed[i] != 0) ? seed[i] : 1;
		seedi %= 2147483647; //max int value
	}
	if (debug)printf("<%d>[%s]\n", seedi, seed);
	srand(seedi);
	bool rep = false;
	for (int id = 0; id < 10; id++)
	{
		int x = 0, y = 0, roomSize = 0;
		do {
			x = rand() % 146 + 2;
			y = rand() % 36 + 2;
			rep = false;
			int range = 0;
			if (x - 10 <= 0)range = x;
			else if (x + 10 >= 140)range = 140 - x;
			else if (y - 10 <= 0)range = y;
			else if (y + 10 >= 40)range = 40 - y;
			if (x - 10 <= 0 || x + 10 >= 140 || y - 10 <= 0 || y + 10 >= 40)
			{
				if (range = 1)roomSize = 1;
				else roomSize = rand() % (range - 1) + 1;
			}
			else roomSize = rand() % (10 - 1) + 1;
			for (int i = 0; i < path.points.size(); i++)
			{
				if (x == path.points[i]->x)rep = true;
				if (y == path.points[i]->y)rep = true;
			}
		} while (rep);
		path.points.push_back(new Point(x, y, id, roomSize));
		if (true);
	}
	for (int i = 0; i < path.points.size(); i++)
	{
		int j = i + 1;
		if (j >= path.points.size()) j = 0;
		makeRoom(path.points[i]->x, path.points[i]->y, path.points[i]->roomSize);
		makePath(seedi, path.points[i]->x, path.points[i]->y, path.points[j]->x, path.points[j]->y);
	}
	for (int i = 0; i < path.points.size(); i++)
	{
		if(debug)std::cout << path.points[i]->id << ":" << path.points[i]->roomSize << "	(" << path.points[i]->x << ", " << path.points[i]->y << ")" << std::endl;
		map[path.points[i]->y][path.points[i]->x] = 2;
	}
	if (debug) {
		system("pause");
	}
	system("cls");
	printf("\n");
	render();
}