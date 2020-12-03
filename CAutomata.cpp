#include "main.h"

//Counts how many tiles in the map are of the value a
int tileCount(unsigned char map[40][150], unsigned char a)
{
	int b = 0;
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 150; x++)
		{
			if (map[y][x] == a)
			{
				b++;
			}
		}
	}
	return b;
}

//Counts how many of the neighbours of (x, y) are of type a on the map
int neighbourCount(unsigned char map[40][150], unsigned int x, unsigned int y, unsigned char a)
{
	int b = 0;
	if (map[y - 1][x] == a)b++; //north
	if (map[y + 1][x] == a)b++; //south
	if (map[y - 1][x + 1] == a)b++; //north-east
	if (map[y][x + 1] == a)b++; //east
	if (map[y + 1][x + 1] == a)b++; //south-east
	if (map[y - 1][x - 1] == a)b++; //north-west
	if (map[y][x - 1] == a)b++; //west
	if (map[y + 1][x - 1] == a)b++; //south-west
	return b;
}

//main generation function
void CAutomata::generate()
{
	system("cls");
	char seed[40];
	for (int i = 0; i < 40; i++)seed[i] = 0;
	getSeed(seed);
	
	//unsigned char because int takes up too much memory at this quantity of values
	unsigned char map[40][150]; //6000 tiles
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 150; x++)
		{
			map[y][x] = 0;
		}
	}
	////////////////////////////////////////
	int seedi = 1;
	for (int i = 0; i < strlen(seed); i++)
	{
		seedi *= seed[i];
		seedi %= 2147483647;
	}
	srand(seedi);
	//setting y = 0 and y = 40 to walls
	for (int x = 0; x < 150; x++)
	{
		map[0][x] = 1;
		map[39][x] = 1;
	}
	//setting x = 0 and x = 150 to walls
	for (int y = 0; y < 40; y++)
	{
		map[y][0] = 1;
		map[y][149] = 1;
	}
	//generates random walls until there are 2800 of them
	while(tileCount(map, 1) < 2800)
	{
		int x = rand() % 148 + 1;
		int y = rand() % 38 + 1;
		if (map[y][x] == 0)map[y][x] = 1;
	}
	//showcases the randomly generated, pre-cellular automata map 
	draw(map);
	system("pause");
	system("cls");
	/////////////////////////////////////////
	for (int i = 0; i < 3; i++) //repeat 3 times
	{
		for (int y = 1; y < 38; y++)
		{
			for (int x = 1; x < 148; x++)
			{
				if (neighbourCount(map, x, y, 1) > 4) //if there are more than 4 neighbouring walls set to wall
				{
					map[y][x] = 1;
				}
				if (neighbourCount(map, x, y, 0) > 5)//if there are more than 5 neighbouring floors set to floor
				{
					map[y][x] = 0;
				}
			}
		}
	}
	//render/draw final map
	draw(map);
}

//Ask for and/or Generate a seed
void CAutomata::getSeed(char seed[40])
{
	int len = (int)strlen(seed);
	char input[40];
	printf("Enter a seed (0 for random): ");
	scanf_s("%s", input, 40);
	
	//if input is 0, generate a seed
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

//draws the map using the values given in map
void CAutomata::draw(unsigned char map[40][150])
{
	char chMap[] = {'.', '#', '*'};
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 150; x++)
		{
			printf("%c", chMap[map[y][x]]);
		}
		printf("\n");
	}
}
