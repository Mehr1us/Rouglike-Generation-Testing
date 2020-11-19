
class Tools {
public:
	int valFinder(int map[19][49], int value);
	bool touchingBorder(int x, int y);
	void render(int map[][49], int a, int b);
	void scrambler(int arr[], int l, int c, int loop, char seed[40]);
	int nebCount(int map[][49], int i, int j, int tileno);
	bool noDestination(int map[][49], int i, int j);
};
