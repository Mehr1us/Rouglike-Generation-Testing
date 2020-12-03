# Rouglike-Generation-Testing
my repos for Testing Generation v2.0, clearly not efficient code, just my attempt at rougelike-2d-generation

## [Cellular Automata:](https://github.com/Mehr1us/Rouglike-Generation-Testing/blob/main/CAutomata.cpp)  
Useful for making caves.  
Simple to code.  
4 neighbouring walls, change tile to wall, 5 neighbouring floors change tile to floor.  
http://roguebasin.com/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels  
Too lazy to make pathfinifing to connect up all created rooms.  
["Cellular Automata.png"](https://github.com/Mehr1us/Rouglike-Generation-Testing/blob/main/CAutomata.png)  

## [My Attempt at Path Generation:](https://github.com/Mehr1us/Rouglike-Generation-Testing/blob/main/PathTest.cpp)  
Linear and Non-Linear paths, The non-linear determine which direction to snake in based on whether the current distance  
over the total distance to the second point is greater on the x axis, or the y axis.  
