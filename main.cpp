#include "main.h"

CAutomata cAuto;
PathTest pathT;

void main()
{

	int input;
	printf("Path Generation(0) or Cellular Automata(1)?\n");
	scanf_s("%d", &input);
	switch (input) {
	case 0:pathT.generate(); break;
	case 1:cAuto.generate(); break;
	default:printf("Not a valid option\n");
	}
}