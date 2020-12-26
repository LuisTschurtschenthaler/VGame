#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H


/* Maths */
#define PI					3.14159265359
#define PI2					(PI * 2)


/* General */
#define CARDIAL_POINT_RANGE 22.5
#define MAX_FPS				60


/* Player */
#define RENDER_DISTANCE		8

#define MOUSE_SENSITIVITY   0.075f
#define MOUSE_TIMEOUT		0.2f

#define WALK_SPEED			0.75f
#define FLY_SPEED			2.25f
#define JUMP_DURATION		1.f
#define MAX_BLOCK_REACH_DISTANCE 6


/* Camera */
#define FOV					110.f
#define FOV_SPRINT			(FOV + 10.f)
#define FOV_SPRINT_STEPS	2.f
#define NEAR_PLANE			0.001f
#define FAR_PLANE			1000.f


/* Physics */
#define GRAVITY				9.81


/* World */
#define MIN_LIGHT_LEVEL		0
#define MAX_LIGHT_LEVEL		15

#define BLOCK_SIZE			1.f

#define CHUNK_SECTIONS		16
#define CHUNK_SIZE			16
#define CHUNK_SIZE_R		(CHUNK_SIZE - 1)
#define HALF_CHUNK_SIZE		(CHUNK_SIZE / 2)
#define HALF_CHUNK_SIZE_R	(HALF_CHUNK_SIZE - 1)
#define CHUNK_AREA			(CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VOLUME		(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#define WATER_LEVEL			50
#define BEACH_LEVEL			(WATER_LEVEL + 3)


#endif // WORLD_CONSTANTS_H
