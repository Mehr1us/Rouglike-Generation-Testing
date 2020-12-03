#include <vector>

class Point {
public:
	int x, y;
	int id;
	int roomSize;
	
	//Constructor of a "Point"
	Point(int x, int y, int id, int roomSize);
};

class Path {
public:
	//adjustable list of Points (which contain 4 ints each)
	std::vector<Point*>points;
	Path();
};

//class for calling functions outside of PathTest.cpp
//everything but generate should prob be not in public but in protected
//but I couldn't be bothered
class PathTest {
public:
	void getSeed(char seed[40]);
	void generate();
	void render();
	void makePath(int seed, int x1, int y1, int x2, int y2);
	void makeRoom(int x, int y, int roomSize);
};
