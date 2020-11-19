#include <string.h>
#include <random>
#include <time.h>
#include <math.h>
#include <cfenv>
#include "main.h"

//generates a random value based on the seed, iteration and the number of unsuccessful loops inbetween iterations
int DWCA::seedRand(int c, int loop, char seed[40])
{
	srand((int)seed[c] * (int)seed[c + 1] * 37 * (loop + 1));
	int x = rand();
	if (debug)printf("srand = %d\n", x);
	return x;
}

//checks if the new j value is part of jImp (has repeated a value)
bool DWCA::compJ(int j, int jImp[3])
{
	for (int i = 0; i < 3; i++)
	{

		if (j == jImp[i])return true;
	}
	return false;
}

void DWCA::dwalkca() {
	system("cls");
	Tools tools;
	int map[19][49];
	char seed[40];
	char input[40];
	char buffer[100];
	int randomImp[4] = { 0, 1, 2, 3 };

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
				} while (compJ(j, jImp));
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
	} while (strlen(seed) < 3);

	int a = 19, b = 49; //so I would only have to switch two values after changing the size of map[][];
	//map[y][x]
	//y = columns, x = rows
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			//initialise all "tiles" as "walls"
			map[i][j] = 1;
		}
	}
	int y = (a - 1) / 2;
	int x = (b - 1) / 2;
	map[y][x] = 0; //initialise the middle as floor
	int c = 0; //how many times has the while loop repeated
	int tempPos[2]; //an array to temporarily store the x and y

	//while less than 200 floors
	while (tools.valFinder(map, 0) < 200)
	{
		int r = 0; //randomised int used to determine which direction to move in
		int loop = 0; // how many times has the do while loop looped continously
		do {
			tempPos[0] = y;
			tempPos[1] = x;
			//technically no longer needed since do while no longer loops if destination is floor
			if (tools.noDestination(map, y, x))
			{
				srand(seed[(int)floor(((float)(y + x) * 37) / (loop + 1)) % strlen(seed)]);
				if (debug) { printf("No Dest %d, %d (%d)\n", x, y, tools.valFinder(map, 0)); }
				y = (rand() % 18) + 1;

				if ((int)floor((float)((x - y) * 3 / 11)) < 0)srand(seed[(int)(-1 * floor(((float)(x - y) * 37) / (loop + 1))) % strlen(seed)]);
				else srand(seed[(int)floor(((float)(x - y) * 37) / (loop + 1)) % strlen(seed)]);
				x = (rand() % 48) + 1;
			}
			int r = seedRand(c, loop, seed) % 4; //generates random input
			tools.scrambler(randomImp, 4, c, loop, seed); //scrambles inputs

			//print randomImp every iteration of the do while when debug is on
			if (debug)
			{
				for (int m = 0; m < 4; m++)
				{
					printf("%d ", randomImp[m]);
				}
				printf("\n");
			}

			//pleasingly random
			if (r == randomImp[0])
			{
				x++;
				if (debug) { printf("(x)%d++;\n", x); }
			}
			else if (r == randomImp[1])
			{
				x--;
				if (debug) { printf("(x)%d--;\n", x); }
			}
			else if (r == randomImp[2])
			{
				y++;
				if (debug) { printf("(y)%d++;", y); }
			}
			else if (r == randomImp[3])
			{
				y--;
				if (debug) { printf("(y)%d--;", y); }
			}

			//if x is on or past the edge make it mot be on or past the edge
			if (x >= 48 || x <= 0)
			{
				x += 3;
				float temp = 0;
				temp = (float)x;
				x %= 48 + 1;
				if (temp > 48)
				{
					temp = (int)floor(temp / 48);
					y += (int)temp;
				}
				if (debug) { printf("x = %d", x); }
			}
			//same as above but for y
			if (y >= 18 || y <= 0)
			{
				y += 3;
				y %= 18 + 1;
				if (debug) { printf("y = %d", y); }
			}
			if (debug)printf(" (%d, %d)\n", x, y);
			loop++;

			//send x and y back a step if step is invalid so that it can pick another random direction
			if (tools.touchingBorder(x, y))
			{
				y = tempPos[0];
				x = tempPos[1];
			}
			if (liteDebug && !debug)printf("loop: %d\n", loop);
		} while (tools.touchingBorder(x, y));
		//if new tile is not floor set to floor
		if (map[y][x] != 0 && !tools.touchingBorder(x, y))
		{
			if (debug)printf("New Floor!\n");
			map[y][x] = 2;
		}
		//these stop the same commands from looping and stop the seed from failing
		c++;
		if ((c + 1) > strlen(seed))c = 0;
		seed[r] += r;
		for (int k = 0; k < strlen(seed); k++)
		{
			seed[k] % 128;
		}

		//debug msgs
		if (liteDebug && !debug)
		{
			system("cls");
			printf("%d\n", tools.valFinder(map, 0));
		}
		if (!debug && showGen)
		{
			system("cls");
			tools.render(map, a, b);
		}
		if (debug)
		{
			tools.render(map, a, b);
			printf("\n");
			printf("%d\n", (tools.valFinder(map, 0)));
		}

		//set '!' to '.' after the single iteration as '!' should only the current place of the x, y
		if (map[y][x] == 2)map[y][x] = 0;
	}

	//Cellular Autotmata, if a tile has 5 neighbouring walls it turns into a wall, if it has 5 neighbouring floors it turns into a floor
	int rep = 0;
	while (rep < 2)
	{
		for (int i = 1; i < a - 1; i++)
		{
			for (int j = 1; j < b - 1; j++)
			{
				int neb = tools.nebCount(map, i, j, 0);
				int nebW = tools.nebCount(map, i, j, 1);
				if (nebW >= 5)map[i][j] = 1;
				if (neb >= 5)map[i][j] = 0;
			}
		}
		rep++;
	}

	printf("\n\n");
	//render
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