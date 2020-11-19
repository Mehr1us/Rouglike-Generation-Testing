#include "main.h"
#include <time.h>
#include <string.h>
#include <random>

//counts how many val exist in grid[][]
int Grid::gridCount(int grid[3][8], int val)
{
	int x = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (grid[i][j] == val)
			{
				x++;
			}
		}
	}

	return x;
}

void Grid::gridGen()
{
	system("cls");
	DWCA dwca;
	Tools tools;
	int map[19][49];
	int grid[3][8];
	int rooms[15][8]; //rooms[no. of room][x, y, x1, y1, x2, y2, x3, y3]
	int a = 19;
	int b = 49;
	char seed[40];
	char input[40];
	char buffer[100];
	
	//initialise int arrays with values that we won't be using
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			grid[i][j] = 0;
		}
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			rooms[i][j] = -1;
		}
	}
	//copied from DWCA
	do {
		printf("Enter a seed of at least 3 characters (0 for random seed): ");
		gets_s(buffer, sizeof(buffer));
		gets_s(input, sizeof(input));

		//if input is 0, generate a 40 value long seed
		if (!strcmp(input, "0"))
		{
			int jImp[3] = { -1, -1, -1 };
			for (int i = 0; i < 40; i++)
			{
				int j = 0;
				int k = i % 21;
				do {
					srand((unsigned int)time(NULL) * (1 + k));
					j = rand() % 4747;
					if (debug)printf("j:%d ", j);
				} while (dwca.compJ(j, jImp));
				jImp[i % 3] = j;
				if (debug)printf("j made\n");
				srand((int)(ldexp(1 + j, k)) % 4747); //arbitrary to be distinct but repeatable when using seeds
				if (debug)printf("srand = %d \n", (int)(ldexp(1 + j, k)) % 4747);
				seed[i] = (rand() + j) % 128;
				if (debug || liteDebug)printf("%d.%d|", i, (rand() + i) % 128);
			}
		}
		//if input isn't 0 copy it to seed
		else strcpy_s(seed, input);
		//having two repeating first characters can cause problems because of my arbitrary way of getting a seed
		if (seed[0] == seed[1])seed[1]++;
		if (seed[0] == seed[2])seed[2]++;
	} while (strlen(seed) < 3);//*/

	//3x8 5x5 squares
	//sets values to some parts of map[][] later to be put through a string as the place of the character
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			map[i][j] = 0;
		}
		for (int j = 0; j < b; j++)
		{
			if (i % 6 == 0)
			{
				map[i][j] = 6;
			}
			else if (j % 6 == 0)
			{
				map[i][j] = 6;
			}
			if (i == 0 || i == 18)
			{
				map[i][j] = 3;
			}
			else if (j == 0 || j == 48)
			{
				map[i][j] = 4;
			}
		}
	}//*/
	map[0][0] = 7;
	map[0][48] = 8;
	map[18][0] = 9;
	map[18][48] = 10;
	
	//based on the seed "randomly" assigns a "random" amount of grid tiles to be occupied with rooms
	int c = 0, d = 0;
	if(debug)printf("%d\n", (unsigned int)seed);
	srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1));
	int n = rand() % (15 - 7) + 7;
	if(debug)printf("%d\n", n);
	do {
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1));
				if (debug)printf("%d\n", (unsigned int)seed * (c + 1) * (d + 1));
				int prob = rand() % 100;
				if(debug)printf("%d, %d (%d,%d)\n", prob, gridCount(grid, 1), x, y);
				if (prob > 70 && gridCount(grid, 1) != n)
				{
					grid[y][x] = 1; //mark grid as occupied
				}
				c++;
				if ((c + 1) > strlen(seed))c = 0;
			}
		}
		d++;
	} while (gridCount(grid, 1) != n);
	//print out grid for debug
	if (debug) {
		printf("Grid is Occupied\n");
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				printf("%d", grid[i][j]);
			}
			printf("\n");
		}
	}
	
	//based on the seed "randomly" creates the square rooms inside the respective grid tiles
	int c1 = 0, d1 = 0; int n0 = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (grid[y][x] == 1)
			{
				int e = 0;
				srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1) * (e + 1));
				if (debug)printf("%d\n", ((unsigned int)seed % 99)* (c1 + 1)* (d1 + 1)* (e + 1));
				//values from 0 - 4 (5)
				
				//(x, y)
				rooms[n0][0] = rand() % 3; //x (0 to 2 (3))
				e++;
				srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1) * (e + 1));
				if (debug)printf("%d\n", ((unsigned int)seed % 99)* (c1 + 1)* (d1 + 1)* (e + 1));
				rooms[n0][1] = rand() % 3; //y
				e++;

				//(x1, y1)
				srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1) * (e + 1));
				if (debug)printf("%d\n", ((unsigned int)seed % 99)* (c1 + 1)* (d1 + 1)* (e + 1));
				switch (rooms[n0][0]) {
				case 0:rooms[n0][2] = 4 - rand() % 3; break; //x1
				case 1:rooms[n0][2] = (rand() % (rooms[n0][0] + 1)) + (4 - rooms[n0][0]); break; //x1 (2 to 4 (3)) // ((rand() % (max - min)) + min)
				case 2:rooms[n0][2] = 4; break; //x1
				}
				e++;
				rooms[n0][3] = rooms[n0][1]; //y1

				//(x2, y2)
				rooms[n0][4] = rooms[n0][0]; //x2

				srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1) * (e + 1));
				if (debug)printf("%d\n", ((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1) * (e + 1)));
				switch (rooms[n0][1]) {
				case 0:rooms[n0][5] = 4 - rand() % 3; break; //y2
				case 1:rooms[n0][5] = (rand() % (rooms[n0][1] + 1)) + (4 - rooms[n0][1]); break; //y2
				case 2:rooms[n0][5] = 4; break; //y2
				}
				e++;

				//(x3, y3)
				rooms[n0][6] = rooms[n0][2]; //x3
				rooms[n0][7] = rooms[n0][5]; //y3
				if (debug){
					for (int i = 0; i < 8; i+=2)
					{
						printf("x%d.%d, y%d.%d\n", i, rooms[n0][i], i, rooms[n0][i + 1]);//rooms[n0][0], rooms[n0][1], rooms[n0][2]
					}
				}
				if ((c1 + 1) > strlen(seed))c1 = 0;
				c1++;
				n0++;
			}
		}
		d1++;
	}

	//prints the rooms[][] grid for debug
	if (debug) {
		printf("Grid is Populated\n");
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				printf("%d", rooms[i][j]);
			}
			printf("\n");
		}
	}
	
	//based on the generated values in grid[][] and rooms[][] changes the values of map[][] to the respective values
	int n1 = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (grid[y][x] == 1)
			{
				for (int z = 0; z < 7; z += 2) // z: 0, 2, 4
				{
					if (debug)printf("z = %d\n", z);
					if ((z % 4) == 0)
					{
						int ax = rooms[n1][z];
						int bx = rooms[n1][z + 2];
						int diff_x = bx - (ax + 1);
						if (debug)printf("%d - (%d + 1) = %d\n", bx, ax, diff_x);
						for (int i = 1; i <= diff_x; i++)
						{
							map[((1 + rooms[n1][z + 1]) + (6 * y))][((1 + rooms[n1][z]) + (6 * x) + i)] = 3;
							if (debug)printf("%d!%d(%d, %d)\n", z, i, ((1 + rooms[n1][z + 1]) + (6 * y)), ((1 + rooms[n1][z]) + (6 * x) + i));
						}
					}
					if ((z == 0) || (z == 2))
					{
						int ay = rooms[n1][z + 1];
						int by = rooms[n1][z + 5];
						int diff_y = by - (ay + 1);
						if (debug)printf("%d - (%d + 1) = %d\n", by, ay, diff_y);
						for (int i = 1; i <= diff_y; i++)
						{
							map[((1 + rooms[n1][z + 1]) + (6 * y) + i)][((1 + rooms[n1][z]) + (6 * x))] = 4;
							if (debug)printf("%d!%d(%d, %d)\n", z, i, ((1 + rooms[n1][z + 1]) + (6 * y) + i), ((1 + rooms[n1][z]) + (6 * x)));
						}
					}

					map[((1 + rooms[n1][z + 1]) + (6 * y))][((1 + rooms[n1][z]) + (6 * x))] = 5;
				}
				n1++;
				
				//if showGen is true print the map every step (not tested)
				if (!debug && showGen)
				{
					system("cls");
					tools.render(map, a, b);
				}
			}
		}
	}
	printf("\n\n");
	//renders/prints the map using the render() function from Tools.cpp declared in Tools.h
	tools.render(map, a, b);
}


/*
					   88
					   ""

8b,dPPYba,  ,adPPYYba, 88 8b,dPPYba,
88P'    "8a ""     `Y8 88 88P'   `"8a
88       d8 ,adPPPPP88 88 88       88
88b,   ,a8" 88,    ,88 88 88       88
88`YbbdP"'  `"8bbdP"Y8 88 88       88
88
88
*/
