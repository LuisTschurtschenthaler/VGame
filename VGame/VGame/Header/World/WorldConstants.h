#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H

#define PI 3.14159265359
#define PI2 PI * 2
#define SUNRISE_TIME 6
#define GRAVITY 1 //9.81

constexpr float BLOCK_SIZE		= 1.f;

constexpr int RENDER_DISTANCE	= 6;

constexpr int CHUNK_SECTIONS	= 8;
constexpr int CHUNK_SIZE		= 32;
constexpr int CHUNK_AREA		= CHUNK_SIZE * CHUNK_SIZE;
constexpr int CHUNK_VOLUME		= CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

constexpr int WATER_LEVEL		= 50;
constexpr int BEACH_LEVEL		= WATER_LEVEL + 3;

#endif // WORLD_CONSTANTS_H
