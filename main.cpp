#include "main.h"

//classes (look at CAutomata.h and PathTest.h respectively)
CAutomata cAuto;
PathTest pathT;

void main()
{
	//ask for which Generation to showcase/test
	int input;
	printf("Path Generation(0) or Cellular Automata(1)?\n");
	scanf_s("%d", &input);
	switch (input) {
	case 0:pathT.generate(); break;
	case 1:cAuto.generate(); break;
	default:printf("Not a valid option\n");
	}
}
