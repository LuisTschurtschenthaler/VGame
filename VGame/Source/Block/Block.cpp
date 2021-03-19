#include "Block.h"
#include "MeshTypes.h"
#include "Constants.h"
#include "AABB.h"

/* Name, BlockBreakingTexture, TEXTURE{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, 
	TexturePixelOffset{ RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK }, MeshType, LL, IsFloraBlock, HasHitbox, IsTransparent, useAmbient, IsRotateable
};*/

Block BlockManager::_blocks[TOTAL_BLOCKS] = {
	Block { "Air",				   255, { 255, 255, 255, 255, 255, 255 }, NONE,   0.f, false, false, true, false },
	Block { "Water",				0,  {   0,   0,   0,   0,   0,   0 }, FLUID,  0.f, false, false, false, false },
	Block { "Lava",					1,  {   1,   1,   1,   1,   1,   1 }, FLUID, 15.f, false, false, false, false },
	Block { "Snow",					2,  {   2,   2,   2,   2,   2,   2 }, SOLID,  0.f, false, true, false, true },
	Block { "Dirt",					3,  {   3,   3,   3,   3,   3,   3 }, SOLID,  0.f, false, true, false, true },
	Block { "Grass",				5,  {   4,   4,   5,   3,   4,   4 }, SOLID,  0.f, false, true, false, true },
	Block { "Stone",				6,  {   6,   6,   6,   6,   6,   6 }, SOLID,  0.f, false, true, false, true },
	Block { "Oak log",				7,  {   7,   7,   8,   8,   7,   7 }, SOLID,  0.f, false, true, false, true },
	Block { "Oak leave",			9,  {   9,   9,   9,   9,   9,   9 }, SOLID,  0.f, true, true, false, true },
	Block { "Sand",					10, {  10,  10,  10,  10,  10,  10 }, SOLID,  0.f, false, true, false, true },
	Block { "Sandstone",			11, {  11,  11,  11,  11,  11,  11 }, SOLID,  0.f, false, true, false, true },
	Block { "Smirk",				12, {  12,  12,  12,  12,  12,  12 }, SOLID,  0.f, false, true, false, true },
	Block { "Cactus",				13, {  13,  13,  15,  14,  13,  13 }, SOLID,  0.f, true, true, false, false },
	Block { "Gravel",				16, {  16,  16,  16,  16,  16,  16 }, SOLID,  0.f, false, true, false, true },
	Block { "Coal ore",				17, {  17,  17,  17,  17,  17,  17 }, SOLID,  0.f, false, true, false, true },
	Block { "Iron ore",				18, {  18,  18,  18,  18,  18,  18 }, SOLID,  0.f, false, true, false, true },
	Block { "Gold ore",				19, {  19,  19,  19,  19,  19,  19 }, SOLID,  0.f, false, true, false, true },
	Block { "Lapis ore",			20, {  20,  20,  20,  20,  20,  20 }, SOLID,  0.f, false, true, false, true },
	Block { "Diamond ore",			21, {  21,  21,  21,  21,  21,  21 }, SOLID,  0.f, false, true, false, true },
	Block { "Dead bush",			22, { 255, 255, 255, 255,  22,  22 }, FLORA,  0.f, true, false, false, false },
	Block { "Grass",				23, { 255, 255, 255, 255,  23,  23 }, FLORA,  0.f, true, false, false, false },
	Block { "Tall grass",			24, { 255, 255,  24,  25, 255, 255 }, FLORA,  0.f, true, false, false, false },
	Block { "TNT",					27, {  27,  27,  28,  26,  27,  27 }, SOLID,  0.f, false, true, false, true },
	Block { "Dandelion",			29, { 255, 255, 255, 255,  29,  29 }, FLORA,  0.f, true, false, false, false },
	Block { "Poppy",				30, { 255, 255, 255, 255,  30,  30 }, FLORA,  0.f, true, false, false, false },
	Block { "Blue Orchid",			31, { 255, 255, 255, 255,  31,  31 }, SOLID,  0.f, true, false, false, false },
	Block { "Birch log",			32, {  32,  32,  33,  33,  32,  32 }, SOLID,  0.f, false, true, false, true },
	Block { "Birch leave",			34, {  34,  34,  34,  34,  34,  34 }, SOLID,  0.f, true, true, false, true },
	Block { "Jungle log",			35, {  35,  35,  36,  36,  35,  35 }, SOLID,  0.f, false, true, false, true },
	Block { "Jungle leave",			37, {  37,  37,  37,  37,  37,  37 }, SOLID,  0.f, true, true, false, true },
	Block { "Glass",				38, {  38,  38,  38,  38,  38,  38 }, SOLID,  0.f, false, true, true, false },
	Block { "Oak planks",			39, {  39,  39,  39,  39,  39,  39 }, SOLID,  0.f, false, true, false, true },
	Block { "Birch planks",			40, {  40,  40,  40,  40,  40,  40 }, SOLID,  0.f, false, true, false, true },
	Block { "Jungle planks",		41, {  41,  41,  41,  41,  41,  41 }, SOLID,  0.f, false, true, false, true },
	Block { "Cobblestone",			42, {  42,  42,  42,  42,  42,  42 }, SOLID,  0.f, false, true, false, true },
	Block { "Bedrock",				43, {  43,  43,  43,  43,  43,  43 }, SOLID,  0.f, false, true, false, true },
	Block { "Emerald ore",			44, {  44,  44,  44,  44,  44,  44 }, SOLID,  0.f, false, true, false, true },
	Block { "Mossy cobblestone",	45, {  45,  45,  45,  45,  45,  45 }, SOLID,  0.f, false, true, false, true },
	Block { "Stone bricks",			46, {  46,  46,  46,  46,  46,  46 }, SOLID,  0.f, false, true, false, true },
	Block { "Melon",				47, {  47,  47,  48,  48,  47,  47 }, SOLID,  0.f, false, true, false, true },
	Block { "Pumpkin",				49, {  49,  49,  50,  51,  49,  49 }, SOLID,  0.f, false, true, false, true },
	Block { "Craved pumpkin",		52, {  49,  49,  50,  51,  52,  49 }, SOLID,  0.f, false, true, false, true },
	Block { "Jack O Lanern",		53, {  49,  49,  50,  51,  53,  49 }, SOLID, 15.f, false, true, false, true },
	Block { "Craved pumpkin",		54, {  49,  49,  50,  51,  54,  49 }, SOLID,  0.f, false, true, false, true },
	Block { "Jack O Lanern",		55, {  49,  49,  50,  51,  55,  49 }, SOLID, 15.f, false, true, false, true },
	Block { "Diorite",				56, {  56,  56,  56,  56,  56,  56 }, SOLID,  0.f, false, true, false, true },
	Block { "Andesite",				57, {  57,  57,  57,  57,  57,  57 }, SOLID,  0.f, false, true, false, true },
	Block { "Granite",				58, {  58,  58,  58,  58,  58,  58 }, SOLID,  0.f, false, true, false, true },
	Block { "Pink glass",			59, {  59,  59,  59,  59,  59,  59 }, SOLID,  0.f, false, true, true, false },
	
	Block { "Error",				255, { 255, 255, 255, 255, 255, 255 }, SOLID,  0.f, false, true, false, true }
};

const Block& BlockManager::getBlock(const uint8_t& blockID) {
	return _blocks[int(blockID)];
}
