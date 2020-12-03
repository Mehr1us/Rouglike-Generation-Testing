#include <vector>

class Point {
public:
	int x, y;
	int id;
	int roomSize;

	Point(int x, int y, int id, int roomSize);
};

class Path {
public:
	std::vector<Point*>points;
	Path();
};

class PathTest {
public:
	void getSeed(char seed[40]);
	void generate();
	void render();
	void makePath(int seed, int x1, int y1, int x2, int y2);
	void makeRoom(int x, int y, int roomSize);
};