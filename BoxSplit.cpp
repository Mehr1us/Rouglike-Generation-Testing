#include "main.h"

static const int render_width = 49;
static const int render_height = 19;

Box::Box(int x, int y) : x(x), y(y) {
	double xy = (double)x * (double)y * render_width;
	tile = new Tile[xy];
}

void seeed(char seed[40], int c, int d)
{
	srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1)); c++; if ((c + 1) > (int)strlen(seed))c = 0;
}

int lenIdFill(int idList[])
{
	int x = 0; int szid = sizeof(idList); int szid0 = sizeof(idList[0]);
	int lenId = szid / szid0;
	int cpy[20];
	for (int i = 0; i < 20; i++)
	{
		cpy[i] = idList[i];
	}
	for (int i = 0; i < lenId; i++)
	{
		if (cpy[i] > -1 && cpy[i] < 20)x++;
	}
	return x;
}

bool idChecker(class Box* box, int x, int y, int width, int randomId)
{
	int a = (int)(box->tile[x + (y * width)].box_id);
	if (a == randomId) 
	{ 
		if (debug)printf("%d == box->tile[%d+%d*%d].box_id = t \n", a, x, y, width); 
		return true; 
	}
	if(debug)printf(" % d == box->tile[% d + % d * %d].box_id =  f \n", a, x, y, width);
	return false;
}

int randomMid(int max, int min)
{
	int mid = (int)((float)(max - min) * (((rand() % (70 - 30)) + 30))/100);
	return mid;
}

int gridChecker(int grid[20], int val)
{
	int x = 0;
	for (int i = 0; i < 19; i++)
	{
		//printf("%d:%d;", grid[i], val);
		if ((int)grid[i] == val)x++;
	}
	//printf("\n[%d]\n", x);
	return x;
}

bool included(int popIds[20], int id)
{
	for (int i = 0; i < 20; i++)
	{
		if (popIds[i] == id)return true;
	}
	return false;
}

void BoxSplit::Bsplit()
{
	system("cls");
	DWCA dwca;
	char symbmap[] = { '.', '#', '!' ,205,186, 'x', '.' };
	int idList[2][20];
	for (int i = 0; i < 20; i++)
	{
		idList[0][i] = -1;
		idList[1][i] = -1;
	}
	idList[0][0] = 0;
	idList[1][0] = 0;
	char seed[40];
	char buffer[40];
	char input[40];
	int popIds[20];
	bool buf = true;
	//copied from DWCA
	do {
		printf("Enter a seed of at least 3 characters (0 for random seed): ");
		if (buf)gets_s(buffer, sizeof(buffer));
		buf = false;
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
	} while (strlen(seed) < 3);
	//create box
	box = new Box(render_width, render_height);
	int c = 0; int d = 0;
	for (int y = 0; y < render_height; y++)
	{
		for (int x = 0; x < render_width; x++)
		{
			box->tile[x + (y * render_width)].box_id = 0;
			box->tile[x + y * render_width].x = x;
			box->tile[x + y * render_width].y = y;
			box->tile[x + y * render_width].tile_type = 0;
		}
	}
	if (debug)printf("a\n");
	int id = 1;
	do {
		/////////////////////////////////////////////////////////////////////////////////
	reset:
		int loop = 0, randId = -1;
		srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1)); c++;
		int dir = rand() % 2;
		srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1)); c++;
		do {
			randId = idList[dir][rand() % id];
			if (randId >= 0)break;
		} while (true);
	skip:
		int xa = 0, ya = 0, maxx = 0, minx = 50, maxy = 0, miny = 20, midx = 0, midy = 0;
		for (int y = 0; y < render_height; y++)
		{
			for (int x = 0; x < render_width; x++)
			{
				if (box->tile[x + y * render_width].box_id == (randId))
				{
					if (box->tile[x + y * render_width].x > maxx)maxx = box->tile[x + y * render_width].x;
					if (box->tile[x + y * render_width].y > maxy)maxy = box->tile[x + y * render_width].y;
					if (box->tile[x + y * render_width].x < minx)minx = box->tile[x + y * render_width].x;
					if (box->tile[x + y * render_width].y < miny)miny = box->tile[x + y * render_width].y;
				}
			}
		}
		if (debug)printf("randId: %d\nb\n", randId);
		if ((c + 1) > (int)strlen(seed))c = 0;
		bool xplus = false; maxx++;
		bool yplus = false; maxy++;
		if (((double)maxx / 2 - (int)((double)maxx / 2)) != 0) { maxx++; xplus = true; }
		if (((double)maxy / 2 - (int)((double)maxy / 2)) != 0) { maxy++; yplus = true; }
		srand((int)seed[c] * (int)seed[c + 1] * 37 * (d + 1)); c++;
		if (debug)printf("dir: %d\nminx: %d, miny:%d\n", dir, minx, miny);
		if (dir == 0)midx = randomMid(maxx, minx); //vertical split
		if (dir == 1)midy = randomMid(maxy, miny); //horizontal split
		maxx -= xplus; maxy -= yplus;
		if(debug)printf("midx:%d, midy:%d\nmaxx:%d, maxy:%d\nmax-min-mid:(%d, %d)c\n", midx, midy, maxx, maxy, maxx - midx - minx, maxy - midy - miny);
		if (maxx - minx - midx < 4 || maxy - miny - midy < 4 || midy < 4 && dir == 1|| midx < 4 && dir == 0)
		{ 
			if (loop > 10)
			{
				if (debug)printf("Reset!-------------------------------------------------------------------\n");
				goto reset;
			}
			if (debug)printf("Skipped!\n");
			loop++;
			goto skip; 
		}
		for (int y = midy + miny; y < maxy; y++)
		{
			for (int x = midx + minx; x < maxx; x++)
			{
				box->tile[x + (y * render_width)].box_id = id;
			}
		}
		if (maxx - minx - midx >= 7)
		{
			idList[0][id] = id;
			if (debug)printf("id added (0)\n");
		}
		if (midx < 8 && dir == 0)
		{
			idList[0][randId] = -1;
			if (debug)printf("id removed (0)\n");
		}
		if (maxy - miny - midy >= 7)
		{
			idList[1][id] = id;
			if (debug)printf("id added (1)\n");
		}
		if (midy < 8 && dir == 1)
		{
			idList[1][randId] = -1;
			if (debug)printf("id removed (1)\n");
		}
		id++; if (debug)printf("id is now %d\n", id);
		if (debug || showGen)
		{
			printf("d\n");
			for (int y = 0; y < render_height; y++)
			{
				for (int x = 0; x < render_width; x++)
				{
					printf("%c", (char)(box->tile[x + y * render_width].box_id + 97));
				}
				printf("\n");
			}
			for (int z = 0; z < 20; z++)
			{
				printf("%d", idList[0][z]);
				printf("%d\n", idList[1][z]);
			}
			//system("pause");
		}
	} while (id < 20);
	printf("\n");
	for (int y = 0; y < render_height; y++)
	{
		for (int x = 0; x < render_width; x++)
		{
			printf("%c", (char)(box->tile[x + y * render_width].box_id + 97));
		}
		printf("\n");
	}
	//
	//int rooms[17][8];
	//int grid[20];
	//for (int i = 0; i < 20; i++)
	//{
	//	grid[i] = 0;
	//}
	////printf("gr\n");
	//for (int i = 0; i < 17; i++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		rooms[i][j] = -1;
	//	}
	//}
	////printf("rms\n");
	//int c1 = 0, d1 = 0;
	//srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1)); c1++;
	//int n = (int)(rand() % (17 - 13) + 13);
	//int it = 0;
	//do {
	//	for (int i = 0; i < 20; i++)
	//	{
	//		srand((int)seed[c1] * (int)seed[c1 + 1] * 37 * (d1 + 1)); c1++; if ((c1 + 1) > (int)strlen(seed))c1 = 0;
	//		int prob = rand() % 100;
	//		if ((prob > 80) && ((gridChecker(grid, 1)) != n) && !(included(popIds, i)))
	//		{
	//			//printf("!");
	//			grid[i] = 1;
	//			popIds[it] = i;
	//			it++;
	//			/*for (int j = 0; j < 20; j++)
	//			{
	//				printf("%d,", grid[j]);
	//			}
	//			printf("\n");
	//			for (int j = 0; j < 20; j++)
	//			{
	//				printf("%d, ", popIds[j]);
	//			}*/
	//		}
	//		//printf("\ngrp (%d, %d, %d, %d, %d)\n", prob, n, gridChecker(grid, 1), i, included(popIds,i));
	//		int r = rand() % strlen(seed);
	//		seed[r] += r;
	//	}
	//	d++;
	//} while (gridChecker(grid, 1) != n);
	//printf("\nPopulated!\n");
	//int i2 = 0, i3 = 0;
	//int maxx = 0, minx = 100, maxy = 0, miny = 100;
	//for (int i = 0; i < it; i++)
	//{
	//	c1++;
	//	for (int y = 0; y < render_height; y++)
	//	{
	//		for (int x = 0; x < render_width; x++)
	//		{
	//			if (box->tile[x + y * render_width].box_id == popIds[i])
	//			{
	//				if (box->tile[x + y * render_width].x > maxx)maxx = box->tile[x + y * render_width].x;
	//				if (box->tile[x + y * render_width].x < minx)minx = box->tile[x + y * render_width].x;
	//				if (box->tile[x + y * render_width].y > maxy)maxy = box->tile[x + y * render_width].y;
	//				if (box->tile[x + y * render_width].y < miny)miny = box->tile[x + y * render_width].y;
	//			}
	//		}
	//	}
	//	if (i2 <= n)
	//	{
	//		printf("[%d]\nxrange = %d - %d;\nyrange = %d - %d;\n", i, maxx, minx, maxy, miny);
	//		int xrange = maxx - minx;
	//		int yrange = maxy - miny;
	//		seeed(seed, c1, d1);
	//		rooms[i2][0] = rand() % (xrange - 1); //x
	//		seeed(seed, c1, d1);
	//		rooms[i2][1] = rand() % (yrange - 1); //y
	//		seeed(seed, c1, d1);
	//		rooms[i2][2] = xrange - rand() % (xrange - 1 - rooms[i2][0]); //x1
	//		rooms[i2][3] = rooms[i2][1]; //y1
	//		rooms[i2][4] = rooms[i2][0]; //x2
	//		seeed(seed, c1, d1);
	//		rooms[i2][5] = yrange - rand() % (yrange - 1 - rooms[i2][1]); //y2
	//		rooms[i2][6] = rooms[i2][2]; //x3
	//		rooms[i2][7] = rooms[i2][5]; //y3
	//		i2++; d1++;
	//	}
	//	else break;
	//}
	//for (int i = 0; i < it; i++)
	//{
	//	int maxx2 = 0, minx2 = 100, maxy2 = 0, miny2 = 100;
	//	printf("%d, %d\n", i, popIds[i]);
	//	if (popIds[i] != -1)
	//	{
	//		for (int y = 0; y < render_height; y++)
	//		{
	//			for (int x = 0; x < render_width; x++)
	//			{
	//				if (box->tile[x + y * render_width].box_id == popIds[i])
	//				{
	//					//printf("%d (%d,%d),", box->tile[x + y * render_width].box_id,x,y);
	//					if (box->tile[x + y * render_width].x > maxx2)maxx2 = box->tile[x + y * render_width].x;
	//					if (box->tile[x + y * render_width].y > maxy2)maxy2 = box->tile[x + y * render_width].y;
	//					if (box->tile[x + y * render_width].x < minx2)minx2 = box->tile[x + y * render_width].x;
	//					if (box->tile[x + y * render_width].y < miny2)miny2 = box->tile[x + y * render_width].y;
	//				}
	//			}
	//			//printf("\n");
	//		}
	//		printf("maxx = %d, minx = %d\nmaxy = %d, miny = %d\n", maxx2, minx2, maxy2, miny2);
	//		for (int z = 0; z < 7; z += 2) // z: 0, 2, 4, 6
	//		{
	//			printf("(%d, %d) : (%d, %d)\n", ((1 + rooms[i][z]) + (minx2)), (((1 + rooms[i][z + 1]) + (miny2))), rooms[i][z], rooms[i][z + 1]);
	//			box->tile[((1 + rooms[i][z]) + (minx2)) + (((1 + rooms[i][z + 1]) + (miny2)) * render_width)].tile_type = 5;
	//		}
	//		if (debug || showGen)
	//		{
	//			for (int ya = 0; ya < render_height; ya++)
	//			{
	//				for (int xa = 0; xa < render_width; xa++)
	//				{
	//					char temp = symbmap[box->tile[xa + ya * render_width].tile_type];
	//					printf("%c", temp);
	//				}
	//				printf("\n");
	//			}
	//		}
	//	}
	//}
	//printf("d1\n");
	//for (int i = 0; i < 20; i++)
	//{
	//	printf("%d,",popIds[i]);
	//}
	//printf("\n");

	////if (debug || showGen)
	//{
	//	printf("	");
	//	for (int xa = 0; xa < render_width; xa++)
	//	{
	//		if (xa <= 25)printf("%c", (char)xa + 65);
	//		else printf("%c", (char)xa + 71);
	//	}
	//	printf("\n");
	//	for (int ya = 0; ya < render_height; ya++)
	//	{
	//		printf("%d	", ya);
	//		for (int xa = 0; xa < render_width; xa++)
	//		{
	//			char temp;
	//			if (box->tile[xa + ya * render_width].tile_type == 0)temp = box->tile[xa + ya * render_width].box_id + 97;
	//			else temp = symbmap[box->tile[xa + ya * render_width].tile_type];
	//			printf("%c", temp);
	//		}
	//		printf("\n");
	//	}
	//}
	//for (int y = 0; y < render_height; y++)
	//{
	//	for (int x = 0; x < render_width; x++)
	//	{
	//		//
	//	}
	//}
	////tools.render(map, 19, 49);
}

//for (int y = 0; y < render_height; y++)
//{
//	for (int x = 0; x < render_width; x++)
//	{
//		//
//	}
//}