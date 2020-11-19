#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>
#include <math.h>
#include <cfenv>

//change to true to see debug msgs
static const bool debug = false;
static const bool liteDebug = false; // only if debug = false;
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

bool touchingBorder(int x, int y)
{
	if (x == 0 || x == 48)return true;
	if (y == 0 || y == 18)return true;
	return false;
}

void render(int map[][49], int a, int b)
{
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

//count num of neighbours that are floors
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

int seedRand(int c, int loop, char seed[40])
{
	srand((int)seed[c] * (int)seed[c + 1] * 37 * (loop + 1));
	int x = rand();
	if (debug)printf("srand = %d\n", x);
	return x;
}

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
					if (j == HUGE_VAL)
					if (debug)printf("j:%d ", j);
				} while (compJ(j, jImp));
				jImp[i % 3] = j;
				if (debug)printf("j made\n");
				srand((int)(ldexp(1 + j, k)) % 4747);
				if(debug)printf("srand = %d \n", (int)(ldexp(1 + j, k)) % 4747);
				seed[i] = (rand() + j) % 128;
				if (debug || liteDebug)printf("%d.%d|", i, (rand() + i) % 128);
			}
		}
		else strcpy_s(seed, input);
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
			map[i][j] = 1;
		}
	}
	int y = (a - 1) / 2; int x = (b - 1) / 2;
	map[y][x] = 0; int c = 0;
	int tempPos[2];
	while (valFinder(map, 0) < 200)
	{
		int r = 0;
		int loop = 0;
		do {
			tempPos[0] = y;
			tempPos[1] = x;
			if (noDestination(map, y, x))
			{
				srand(seed[(int)floor(((float)(y + x) * 37) / (loop + 1)) % strlen(seed)]);
				if (debug) { printf("No Dest %d, %d (%d)\n", x, y, valFinder(map, 0)); }
				y = (rand() % 18) + 1;
				if ((int)floor((float)((x - y) * 3 / 11)) < 0)srand(seed[(int)(-1 * floor(((float)(x - y) * 37) / (loop + 1))) % strlen(seed)]);
				else srand(seed[(int)floor(((float)(x - y) * 37) / (loop + 1)) % strlen(seed)]);
				x = (rand() % 48) + 1;
			}
			int r = seedRand(c, loop, seed) % 4;
			scrambler(randomImp, 4, c, loop, seed);

			if (debug)
			{
				for (int m = 0; m < 4; m++)
				{
					printf("%d ", randomImp[m]);
				}
				printf("\n");
			}
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
			if (y >= 18 || y <= 0)
			{
				y += 3;
				y %= 18 + 1;
				if (debug) { printf("y = %d", y); }
			}
			if (debug)printf(" (%d, %d)\n", x, y);
			loop++;
			if (touchingBorder(x, y))
			{
				y = tempPos[0];
				x = tempPos[1];
			}
			if (liteDebug && !debug)printf("loop: %d\n", loop);
		} while (touchingBorder(x, y));
		if (map[y][x] != 0 && !touchingBorder(x, y))
		{
			if (debug)printf("New Floor!\n");
			map[y][x] = 2;
		}
		c++;
		if ((c + 1) > strlen(seed))c = 0;

		seed[r] += r;
		for (int k = 0; k < strlen(seed); k++)
		{
			seed[k] % 128;
		}
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
		
		if (map[y][x] == 2)map[y][x] = 0;
	}
	/*/
	int mapC[19][49];
	for (int i = 1; i < a - 1; i++)
	{
		for (int j = 1; j < b - 1; j++)
		{
			if (map[i][j] == 1)mapC[i][j] = 2;
			if (map[i][j] == 0)mapC[i][j] = 0;
		}
	}//*/
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
	}//*/
	
	printf("\n\n");
	/*/3x8 5x5 squares
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (i % 6 == 0)
			{
				map[i][j] = 1;
			}
			if (j % 6 == 0)
			{
				map[i][j] = 1;
			}
		}
	}//*/
	render(map, a, b);
	/*/
	int repeat = 1;
	printf("Enter 0 to repeat\n");
	scanf_s("%d", &repeat);
	if (repeat == 0)main();//*/
	return 0;
}