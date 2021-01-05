#include "Block.h"
#include "MeshTypes.h"
#include "Constants.h"
#include "AABB.h"


Block* BlockUtil::blocks[TOTAL_BLOCKS] = {
	/* Name, TEXTURE{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, 
		TexturePixelOffset{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, MeshType, LL, IsFloraBlock, HasHitbox
	*/
	new Block{ "Air",			{ },						{ },				   NONE,  0.f, false, false },
	new Block{ "Water",			{  0,  0,  0,  0,  0,  0 }, { 0, 0, 0, 0, 0, 0 }, FLUID,  0.f, false, false },
	new Block{ "Lava",			{  1,  1,  1,  1,  1,  1 }, { 0, 0, 0, 0, 0, 0 }, FLUID, 15.f, false, false },
	new Block{ "Snow",			{  2,  2,  2,  2,  2,  2 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Dirt",			{  3,  3,  3,  3,  3,  3 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Grass",			{  4,  4,  5,  3,  4,  4 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Stone",			{  6,  6,  6,  6,  6,  6 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Oak log",		{  7,  7,  8,  8,  7,  7 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Oak leave",		{  9,  9,  9,  9,  9,  9 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, true },
	new Block{ "Sand",			{ 10, 10, 10, 10, 10, 10 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Sandstone",		{ 11, 11, 11, 11, 11, 11 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Smirk",			{ 12, 12, 12, 12, 12, 12 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Cactus",		{ 13, 13, 15, 14, 13, 13 }, { 1, 1, 0, 0, 1, 1 }, SOLID,  0.f, true, true },
	new Block{ "Gravel",		{ 16, 16, 16, 16, 16, 16 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Coal ore",		{ 17, 17, 17, 17, 17, 17 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Iron ore",		{ 18, 18, 18, 18, 18, 18 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Gold ore",		{ 19, 19, 19, 19, 19, 19 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Lapis ore",		{ 20, 20, 20, 20, 20, 20 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Diamond ore",	{ 21, 21, 21, 21, 21, 21 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Dead bush",		{ -1, -1, -1, -1, 22, 22 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },
	new Block{ "Grass",			{ -1, -1, -1, -1, 23, 23 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },
	new Block{ "Tall grass",	{ -1, -1, 24, 25, -1, -1 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },
	new Block{ "TNT",			{ 27, 27, 28, 26, 27, 27 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true },
	new Block{ "Dandelion",		{ -1, -1, -1, -1, 29, 29 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },
	new Block{ "Poppy",			{ -1, -1, -1, -1, 30, 30 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },
	new Block{ "Blue Orchid",	{ -1, -1, -1, -1, 31, 31 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false },

	new Block{ "Error",			{ 255, 255, 255, 255, 255, 255 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true }
};