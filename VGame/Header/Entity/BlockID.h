#ifndef BLOCK_TYPE_H
#define BLOCK_TYPE_H

enum BlockID
	: unsigned char {

	AIR = 0,
	WATER,
	LAVA,
	SNOW,
	DIRT,
	GRASS,
	STONE,
	OAK_LOG,
	OAK_LEAVE,
	SAND,
	SANDSTONE,
	SMIRK,
	CACTUS,
	GRAVEL,
	COAL_ORE,
	IRON_ORE,
	GOLD_ORE,
	LAPIS_ORE,
	DIAMOND_ORE,
	DEAD_BUSH,
	GRASS2,
	TALL_GRASS,
	TNT,
	DANDELION,
	POPPY,
	BLUE_ORCHID,
	BIRCH_LOG,
	BIRCH_LEAVE,
	JUNGLE_LOG,
	JUNGLE_LEAVE,
	GLASS,
	OAK_PLANKS,
	BIRCH_PLANKS,
	JUNGLE_PLANKS,
	COBBLESTONE,
	BEDROCK,
	EMERALD_ORE,
	MOSSY_COBBLESTONE,
	STONE_BRICKS,
	MELON,
	PUMPKIN,
	CRAVED_PUMPKIN,
	JACK_O_LANTERN,
	CRAVED_PUMPKIN2,
	JACK_O_LANTERN2,
	DIORITE,
	ANDESITE,
	GRANITE,
	PINK_GLASS,

	ERROR,
	TOTAL_BLOCKS
};

#endif // BLOCK_TYPE_H
