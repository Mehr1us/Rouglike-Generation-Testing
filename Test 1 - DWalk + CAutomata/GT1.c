#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>
#include <math.h>
#include <cfenv>

//change to true to see debug msgs (v.slow)
static const bool debug = false;
static const bool liteDebug = false; // only if debug = false; (not that fast)
static const bool showGen = false; 
/*change showGen to true when debug = false; to see generation each of the 200 steps at a time (slow)
/	
/	To use it is suggested to change output window font to a monospaced one
/	Tools -> Options -> Environment -> Fonts & Colors -,
/	Show Settings: Output Window -,                  <-'
/	Font: "Your Monospaced Font"<-'
/	"Your Monospaced Font" a mono font that you have downloaded
/	I would suggest Roboto Mono
/	
/	
*/

//Checks how many of int value are in the map
int valFinder(int map[19][49], int value)
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
bool touchingBorder(int x, int y)
{
	if (x == 0 || x == 48)return true;
	if (y == 0 || y == 18)return true;
	return false;
}

//draws the map
void render(int map[][49], int a, int b)
{
	//'.' represent floors, '#' represent walls and '!' represents the current x, y pos only seen if debug or showGen = true;
	char symbmap[] = { '.', '#', '!'};
	
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
int nebCount(int map[][49], int i, int j, int tileno)
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
bool noDestination(int map[][49], int i, int j)
{
	int neb = 0;
	if (map[i + 1][j] == 0 || i + 1 == 48)neb++;
	if (map[i - 1][j] == 0 || i - 1 == 0)neb++;
	if (map[i][j + 1] == 0 || j + 1 == 18)neb++;
	if (map[i][j - 1] == 0 || j - 1 == 0)neb++;
	if (neb == 4)return true;
	return false;
}

//generates a random value based on the seed, iteration and the number of unsuccessful loops inbetween iterations
int seedRand(int c, int loop, char seed[40])
{
	srand((int)seed[c] * (int)seed[c + 1] * 37 * (loop + 1));
	int x = rand();
	if (debug)printf("srand = %d\n", x);
	return x;
}

//scrambler my 4 int array based on the "seed"
void scrambler(int arr[], int l, int c, int loop, char seed[40])
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

//checks if the new j value is part of jImp (has repeated a value)
bool compJ(int j, int jImp[3])
{
	for (int i = 0; i < 3; i++)
	{
		if (j == jImp[i])return true;
	}
	return false;
}

int main() 
{
	int map[19][49];
	char seed[40];
	char input[40];
	int randomImp[4] = {0, 1, 2, 3};
	
	do {
		printf("Enter a seed of at least 3 characters (0 for random seed): ");
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
				if(debug)printf("srand = %d \n", (int)(ldexp(1 + j, k)) % 4747);
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
	while (valFinder(map, 0) < 200)
	{
		int r = 0; //randomised int used to determine which direction to move in
		int loop = 0; // how many times has the do while loop looped continously
		do {
			tempPos[0] = y;
			tempPos[1] = x;
			//technically no longer needed since do while no longer loops if destination is floor
			if (noDestination(map, y, x))
			{
				srand(seed[(int)floor(((float)(y + x) * 37) / (loop + 1)) % strlen(seed)]);
				if (debug) { printf("No Dest %d, %d (%d)\n", x, y, valFinder(map, 0)); }
				y = (rand() % 18) + 1;
				
				if ((int)floor((float)((x - y) * 3 / 11)) < 0)srand(seed[(int)(-1 * floor(((float)(x - y) * 37) / (loop + 1))) % strlen(seed)]);
				else srand(seed[(int)floor(((float)(x - y) * 37) / (loop + 1)) % strlen(seed)]);
				x = (rand() % 48) + 1;
			}
			int r = seedRand(c, loop, seed) % 4; //generates random input
			scrambler(randomImp, 4, c, loop, seed); //scrambles inputs
			
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
			if (touchingBorder(x, y))
			{
				y = tempPos[0];
				x = tempPos[1];
			}
			if (liteDebug && !debug)printf("loop: %d\n", loop);
		} while (touchingBorder(x, y));
		//if new tile is not floor set to floor
		if (map[y][x] != 0 && !touchingBorder(x, y))
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
			printf("%d\n", valFinder(map, 0));
		}
		if (!debug && showGen)
		{
			system("cls");
			render(map, a, b);
		}
		if (debug) 
		{
			render(map, a, b);
			printf("\n");
			printf("%d\n", (valFinder(map, 0)));
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
				int neb = nebCount(map, i, j, 0);
				int nebW = nebCount(map, i, j, 1);
				if (nebW >= 5)map[i][j] = 1;
				if (neb >= 5)map[i][j] = 0;
			}
		}
		rep++;
	}
	
	printf("\n\n");
	//render
	render(map, a, b);
	return 0;
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
