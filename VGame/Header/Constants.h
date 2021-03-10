#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H

/* Maths */
constexpr auto PI = 3.14159265359;
constexpr auto PI2 = (PI * 2);


/* General */
constexpr auto CARDIAL_POINT_RANGE = 22.5;
constexpr auto MAX_BLOCK_BREAK_PARTICLES = 15;


/* File system */
constexpr auto PATH_SHADERS = "./Resources/Shaders/";
constexpr auto PATH_TEXTURES = "./Resources/Textures/";
constexpr auto PATH_SCREENSHOTS = "./Screenshots/";


/* Player */
constexpr auto RENDER_DISTANCE = 15;
constexpr auto DESTROY_DISTANCE = (RENDER_DISTANCE + 3);
constexpr auto SPAWN_CHUNKS = (4 * RENDER_DISTANCE * RENDER_DISTANCE);

constexpr auto MOUSE_SENSITIVITY = 0.075f;

constexpr auto WALK_SPEED = 0.8f;
constexpr auto FLY_SPEED = 1.1f;
constexpr auto JUMP_DURATION = 1.f;
constexpr auto BLOCK_BREAK_DURATION = 0.33f;
constexpr auto MAX_BLOCK_REACH_DISTANCE = 5;


/* Camera */
constexpr auto FOV = 110.f;
constexpr auto FOV_SPRINT = (FOV + 15.f);
constexpr auto FOV_SPRINT_STEPS = 2.5f;
constexpr auto FOV_ZOOM = 45.f;
constexpr auto NEAR_PLANE = 0.01f;
constexpr auto FAR_PLANE = 2000.f;


/* Physics */
constexpr auto GRAVITY = 9.81;


/* World */
constexpr auto MIN_LIGHT_LEVEL = 0;
constexpr auto MAX_LIGHT_LEVEL = 15;

constexpr auto BLOCK_SIZE = 1.f;
constexpr auto HALF_BLOCK_SIZE = (BLOCK_SIZE / 2.f);

constexpr auto CHUNK_SIZE = 16;
constexpr auto CHUNK_HEIGHT = 256;
constexpr auto CHUNK_AREA = (CHUNK_SIZE * CHUNK_SIZE);
constexpr auto CHUNK_VOLUME = (CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE);
constexpr auto HALF_CHUNK_SIZE = (CHUNK_SIZE / 2);

constexpr auto WATER_LEVEL = 64;
constexpr auto BEACH_LEVEL = (WATER_LEVEL + 3);

#endif // WORLD_CONSTANTS_H
