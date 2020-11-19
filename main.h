//change to true to see debug msgs
static const bool debug = false;
static const bool liteDebug = false; // only if debug = false;
static const bool showGen = false;
//change showGen to true when debug = false; to see generation each of the 200 steps at a time (slow)

//stops me from needing to include all of these in every .cpp file
//don't add to other header files
#include <stdio.h>
#include "DWCA.h"
#include "Tools.h"
#include "Grid.h"
