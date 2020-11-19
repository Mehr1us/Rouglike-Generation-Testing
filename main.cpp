#include "main.h"
#include <stdlib.h>
Grid grid;
DWCA dwca;
/*
/	
/	To use it is in Visual Studio I suggest to change output window font to a monospaced one
/	Tools -> Options -> Environment -> Fonts & Colors -,
/	Show Settings: Output Window -,                  <-'
/	Font: "Your Monospaced Font"<-'
/	"Your Monospaced Font" a mono font that you have downloaded
/	I would suggest Roboto Mono
/	
/	
*/

int main() 
{
	//asks which algorithm to use
	int choice;
	do { 
		printf("Pick Generation Method\n0 = Grid Generation\n1 = DWalk + CAutomata\n");
		scanf_s("%d", &choice);
	} while (choice != 0 && choice != 1);
	
	if (choice == 0)grid.gridGen();
	else if (choice == 1)dwca.dwalkca();
}
