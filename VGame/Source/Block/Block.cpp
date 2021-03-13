#include "Block.h"
#include "MeshTypes.h"
#include "Constants.h"
#include "AABB.h"

/* Name, BlockBreakingTexture, TEXTURE{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, 
	TexturePixelOffset{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, MeshType, LL, IsFloraBlock, HasHitbox, IsTransparent, useAmbient, IsRotateable
};*/

Block BlockManager::_blocks[TOTAL_BLOCKS] = {
	Block { "Air",					-1, { -1, -1, -1, -1, -1, -1 }, { 0, 0, 0, 0, 0, 0 }, NONE,   0.f, false, false, true, false },
	Block { "Water",				0,  {  0,  0,  0,  0,  0,  0 }, { 0, 0, 0, 0, 0, 0 }, FLUID,  0.f, false, false, false, false },
	Block { "Lava",					1,  {  1,  1,  1,  1,  1,  1 }, { 0, 0, 0, 0, 0, 0 }, FLUID, 15.f, false, false, false, false },
	Block { "Snow",					2,  {  2,  2,  2,  2,  2,  2 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Dirt",					3,  {  3,  3,  3,  3,  3,  3 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Grass",				5,  {  4,  4,  5,  3,  4,  4 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Stone",				6,  {  6,  6,  6,  6,  6,  6 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Oak log",				7,  {  7,  7,  8,  8,  7,  7 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Oak leave",			9,  {  9,  9,  9,  9,  9,  9 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, true, false, true },
	Block { "Sand",					10, { 10, 10, 10, 10, 10, 10 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Sandstone",			11, { 11, 11, 11, 11, 11, 11 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Smirk",				12, { 12, 12, 12, 12, 12, 12 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Cactus",				13, { 13, 13, 15, 14, 13, 13 }, { 1, 1, 0, 0, 1, 1 }, SOLID,  0.f, true, true, false, false },
	Block { "Gravel",				16, { 16, 16, 16, 16, 16, 16 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Coal ore",				17, { 17, 17, 17, 17, 17, 17 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Iron ore",				18, { 18, 18, 18, 18, 18, 18 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Gold ore",				19, { 19, 19, 19, 19, 19, 19 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Lapis ore",			20, { 20, 20, 20, 20, 20, 20 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Diamond ore",			21, { 21, 21, 21, 21, 21, 21 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Dead bush",			22, { -1, -1, -1, -1, 22, 22 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "Grass",				23, { -1, -1, -1, -1, 23, 23 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "Tall grass",			24, { -1, -1, 24, 25, -1, -1 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "TNT",					27, { 27, 27, 28, 26, 27, 27 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Dandelion",			29, { -1, -1, -1, -1, 29, 29 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "Poppy",				30, { -1, -1, -1, -1, 30, 30 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "Blue Orchid",			31, { -1, -1, -1, -1, 31, 31 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, false, false, false },
	Block { "Birch log",			32, { 32, 32, 33, 33, 32, 32 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Birch leave",			34, { 34, 34, 34, 34, 34, 34 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, true, false, true },
	Block { "Jungle log",			35, { 35, 35, 36, 36, 35, 35 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Jungle leave",			37, { 37, 37, 37, 37, 37, 37 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, true, true, false, true },
	Block { "Glass",				38, { 38, 38, 38, 38, 38, 38 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, true, false },
	Block { "Oak planks",			39, { 39, 39, 39, 39, 39, 39 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Birch planks",			40, { 40, 40, 40, 40, 40, 40 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Jungle planks",		41, { 41, 41, 41, 41, 41, 41 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Cobblestone",			42, { 42, 42, 42, 42, 42, 42 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Bedrock",				43, { 43, 43, 43, 43, 43, 43 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Emerald ore",			44, { 44, 44, 44, 44, 44, 44 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Mossy cobblestone",	45, { 45, 45, 45, 45, 45, 45 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Stone bricks",			46, { 46, 46, 46, 46, 46, 46 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Melon",				47, { 47, 47, 48, 48, 47, 47 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Pumpkin",				49, { 49, 49, 50, 51, 49, 49 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Craved pumpkin",		52, { 49, 49, 50, 51, 52, 49 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Jack O Lanern",		53, { 49, 49, 50, 51, 53, 49 }, { 0, 0, 0, 0, 0, 0 }, SOLID, 15.f, false, true, false, true },
	Block { "Craved pumpkin",		54, { 49, 49, 50, 51, 54, 49 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Jack O Lanern",		55, { 49, 49, 50, 51, 55, 49 }, { 0, 0, 0, 0, 0, 0 }, SOLID, 15.f, false, true, false, true },
	Block { "Diorite",				56, { 56, 56, 56, 56, 56, 56 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Andesite",				57, { 57, 57, 57, 57, 57, 57 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Granite",				58, { 58, 58, 58, 58, 58, 58 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true },
	Block { "Pink glass",			59, { 59, 59, 59, 59, 59, 59 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, true, false },
	
	Block { "Error",				255, { 255, 255, 255, 255, 255, 255 }, { 0, 0, 0, 0, 0, 0 }, SOLID,  0.f, false, true, false, true }
};

const Block& BlockManager::getBlock(const uint8_t& blockID) {
	return _blocks[int(blockID)];
}
