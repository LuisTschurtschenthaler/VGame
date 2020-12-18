#include "Block.h"
#include "MeshTypes.h"
#include "WorldConstants.h"
#include "AABB.h"


Block* BlockUtil::blocks[AMOUNT_OF_BLOCKS] = {
	// Name, { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, texturePixelOffset, LL, meshType, isTransparent, hasHitbox
	new Block{ "",				{ },						0.f,  0.f,    -1, true, false  },
	new Block{ "Water",			{  0,  0,  0,  0,  0,  0 }, 0.f,  0.f, FLUID, true, false  },
	new Block{ "Lava",			{  1,  1,  1,  1,  1,  1 }, 0.f, 15.f, FLUID, true, false  },
	new Block{ "Snow",			{  2,  2,  2,  2,  2,  2 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Dirt",			{  3,  3,  3,  3,  3,  3 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Grass",			{  4,  4,  5,  3,  4,  4 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Stone",			{  6,  6,  6,  6,  6,  6 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Oak log",		{  7,  7,  8,  8,  7,  7 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Oak leave",		{  9,  9,  9,  9,  9,  9 }, 0.f,  0.f, FLORA, false, true  },
	new Block{ "Sand",			{ 10, 10, 10, 10, 10, 10 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Sandstone",		{ 11, 11, 11, 11, 11, 11 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Smirk",			{ 12, 12, 12, 12, 12, 12 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Cactus",		{ 13, 13, 15, 14, 13, 13 }, 1.f,  0.f, SOLID, false, true  },
	new Block{ "Gravel",		{ 16, 16, 16, 16, 16, 16 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Coal ore",		{ 17, 17, 17, 17, 17, 17 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Iron ore",		{ 18, 18, 18, 18, 18, 18 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Gold ore",		{ 19, 19, 19, 19, 19, 19 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Lapis ore",		{ 20, 20, 20, 20, 20, 20 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Diamond ore",	{ 21, 21, 21, 21, 21, 21 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Dead bush",		{ -1, -1, -1, -1, 22, 22 }, 0.f,  0.f, FLORA, false, false },
	new Block{ "Grass",			{ -1, -1, -1, -1, 23, 23 }, 0.f,  0.f, FLORA, false, false },
	new Block{ "Tall grass",	{ -1, -1, 24, 25, -1, -1 }, 0.f,  0.f, FLORA, false, false },
	new Block{ "TNT",			{ 27, 27, 28, 26, 27, 27 }, 0.f,  0.f, SOLID, false, true  },
	new Block{ "Dandelion",		{ -1, -1, -1, -1, 29, 29 }, 0.f,  0.f, FLORA, false, false },
	new Block{ "Poppy",			{ -1, -1, -1, -1, 30, 30 }, 0.f,  0.f, FLORA, false, false },
	new Block{ "Blue Orchid",	{ -1, -1, -1, -1, 31, 31 }, 0.f,  0.f, FLORA, false, false },

	new Block{ "Error",			{ 255, 255, 255, 255, 255, 255 },  0.f,  0.f, SOLID, false, true }
};
