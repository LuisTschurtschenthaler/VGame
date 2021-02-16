#ifndef WORLD_H
#define WORLD_H

class Skybox;
class Player;
class ChunkManager;
class WorldGenerator;
class ParticleSystem;


class World {

public:
	static WorldGenerator* worldGenerator;
	static bool gravityEnabled, disposed;

private:
	static Skybox* _skybox;
	static Player* _player;
	static ChunkManager* _chunkManager;
	static ParticleSystem* _particleSystem;


public:
	World();
	~World();

	void update();
	void draw();
	
	static Player& getPlayer() { return *_player; }
	static ChunkManager& getChunkManager() { return *_chunkManager; }
	static ParticleSystem& getParticleSystem() { return *_particleSystem; }

};

#endif // WORLD_H
