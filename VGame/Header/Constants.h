#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H

/* Maths */
#define PI							3.14159265359
#define PI2							(PI * 2)


/* General */
#define CARDIAL_POINT_RANGE			22.5
#define MAX_BLOCK_BREAK_PARTICLES	50


/* File system */
#define PATH_SHADERS				"./Resources/Shaders/"
#define PATH_TEXTURES				"./Resources/Textures/"


/* Player */
#define RENDER_DISTANCE				10
#define DESTROY_DISTANCE			(RENDER_DISTANCE + 5)
#define SPAWN_CHUNKS				(4 * RENDER_DISTANCE * RENDER_DISTANCE)

#define MOUSE_SENSITIVITY			0.075f
#define MOUSE_TIMEOUT				0.05f

#define WALK_SPEED					0.8f
#define FLY_SPEED					3.f
#define JUMP_DURATION				1.f
#define BLOCK_BREAK_DURATION		0.33f
#define MAX_BLOCK_REACH_DISTANCE	6


/* Camera */
#define FOV							110.f
#define FOV_SPRINT					(FOV + 15.f)
#define FOV_SPRINT_STEPS			3.f
#define FOV_ZOOM					45.f
#define NEAR_PLANE					0.001f
#define FAR_PLANE					500.f


/* Physics */
#define GRAVITY						9.81


/* World */
#define MIN_LIGHT_LEVEL				0
#define MAX_LIGHT_LEVEL				15

#define BLOCK_SIZE					1.f
#define HALF_BLOCK_SIZE				(BLOCK_SIZE / 2.f)

#define CHUNK_SIZE					16
#define CHUNK_HEIGHT				256
#define CHUNK_AREA					(CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VOLUME				(CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE)
#define HALF_CHUNK_SIZE				(CHUNK_SIZE / 2)

#define WATER_LEVEL					64
#define BEACH_LEVEL					(WATER_LEVEL + 3)

#endif // WORLD_CONSTANTS_H
