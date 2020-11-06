#include "Block.h"
#include "MeshTypes.h"


Block* BlockUtil::blocks[AMOUNT_OF_BLOCKS] = {
	// Name, { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, LL, meshType, isTransparent, lightCanPass, hasHitbox
	new Block{ "",			{ },						0.f,    -1, true,  true, false },
	new Block{ "Water",		{  0,  0,  0,  0,  0,  0 }, 0.f, FLUID, true, true, false },
	new Block{ "Lava",		{  1,  1,  1,  1,  1,  1 }, 1.f, FLUID, true, false, false },
	new Block{ "Snow",		{  2,  2,  2,  2,  2,  2 }, 0.f, SOLID, false, false, true },
	new Block{ "Dirt",		{  3,  3,  3,  3,  3,  3 }, 0.f, SOLID, false, false, true },
	new Block{ "Grass",		{  4,  4,  5,  3,  4,  4 }, 0.f, SOLID, false, false, true },
	new Block{ "Stone",		{  6,  6,  6,  6,  6,  6 }, 0.f, SOLID, false, false, true },
	new Block{ "Oak log",   {  7,  7,  8,  8,  7,  7 }, 0.f, SOLID, false, false, true },
	new Block{ "Oak leave", {  9,  9,  9,  9,  9,  9 }, 0.f, FLORA, false, false, true },
	new Block{ "Sand",		{ 10, 10, 10, 10, 10, 10 }, 0.f, SOLID, false, false, true },
	new Block{ "Sandstone", { 11, 11, 12, 12, 11, 11 }, 0.f, SOLID, false, false, true },
	new Block{ "Cactus",    { 13, 13, 15, 14, 13, 13 }, 0.f, SOLID, false, false, true },
	new Block{ "Gravel",    { 16, 16, 16, 16, 16, 16 }, 0.f, SOLID, false, false, true }
};
