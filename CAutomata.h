//a class for CAutomata so I could call the functions from outside of CAutomata.cpp
class CAutomata {
public:
	void generate();
	void getSeed(char seed[40]);
	void draw(unsigned char map[40][150]);
};
