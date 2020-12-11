#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H


/* Maths */
#define PI					3.14159265359
#define PI2					(PI * 2)


/* General */
#define CARDIAL_POINT_RANGE 22.5
#define MAX_FPS				60


/* Player */
#define RENDER_DISTANCE		6

#define  MOUSE_SENSITIVITY  0.075f
#define  WALK_SPEED			0.01f
#define  FLY_SPEED			0.015f
#define  JUMP_DURATION		1.f


/* Camera */
#define FOV_MIN				50.f
#define FOV_MAX				110.f
#define NEAR_PLANE			0.001f
#define FAR_PLANE			1000.f


/* Physics */
#define GRAVITY				9.81


/* World */
#define MIN_LIGHT_LEVEL		0
#define MAX_LIGHT_LEVEL		10

#define BLOCK_SIZE			1.f

#define CHUNK_SECTIONS		8
#define CHUNK_SIZE			32
#define CHUNK_SIZE_R		(CHUNK_SIZE - 1)
#define HALF_CHUNK_SIZE		(CHUNK_SIZE / 2)
#define HALF_CHUNK_SIZE_R	(HALF_CHUNK_SIZE - 1)
#define CHUNK_AREA			(CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VOLUME		(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#define WATER_LEVEL			50
#define BEACH_LEVEL			(WATER_LEVEL + 3)


#endif // WORLD_CONSTANTS_H
