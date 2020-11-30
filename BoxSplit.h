
struct Tile {

	int tile_type;
	int box_id;
	int x;
	int y;
};

class Box {
public:
	int x; int y;
	Box(int x, int y);
	//~Box();
	Tile* tile;
};

class BoxSplit {
public:
	void Bsplit();
	Box *box;
};