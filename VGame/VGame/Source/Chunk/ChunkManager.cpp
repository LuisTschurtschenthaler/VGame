#include "ChunkManager.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Constants.h"
#include "Game.h"
#include "Texture.h"
#include "TextureAtlas.h"


ChunkManager::ChunkManager() {
	_solidShader = new Shader("ChunkShader/solid_vert.glsl", "ChunkShader/solid_frag.glsl");
	_waterShader = new Shader("ChunkShader/fluid_vert.glsl", "ChunkShader/fluid_frag.glsl");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);

	_chunkGenerationThread = new std::thread(&ChunkManager::_generateChunks, this);
}

ChunkManager::~ChunkManager() {

}


void ChunkManager::update() {
	_solidShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_solidShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_solidShader->setMat4("view", World::getPlayer().camera->getView());
	_solidShader->setFloat("dayTime", Game::dayTime);
	_solidShader->setVec3("cameraPosition", World::getPlayer().position);

	_waterShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_waterShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_waterShader->setMat4("view", World::getPlayer().camera->getView());
	_waterShader->setFloat("dayTime", Game::dayTime);
	_waterShader->setVec3("cameraPosition", World::getPlayer().position);
}

void ChunkManager::draw() {
	int playerX = int(World::getPlayer().position.x / CHUNK_SIZE),
		playerZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	for(auto& it : _chunks) {
		if(!it.second->meshesGenerated)
			continue;

		if(it.first.x < playerX - RENDER_DISTANCE || it.first.x > playerX + RENDER_DISTANCE ||
		   it.first.z < playerZ - RENDER_DISTANCE || it.first.z > playerZ + RENDER_DISTANCE) {

			/* Unload chunk and save to file, just if chunk changed */
			//FileManager::save(*it.second);

		}
		else {
			// Draw solid
			_solidShader->bind();
			_textureAtlas->getTexture().bind();
			it.second->drawSolid();
			_solidShader->unbind();
			
			_waterShader->bind();
			_textureAtlas->getTexture().bind();
			it.second->drawFluid();
			_waterShader->unbind();
		}
	}
}

void ChunkManager::getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks) {
	nearbyChunks[CHUNK_RIGHT] = getChunk({ coord.x + 1, coord.z });
	nearbyChunks[CHUNK_LEFT]  = getChunk({ coord.x - 1, coord.z });
	nearbyChunks[CHUNK_FRONT] = getChunk({ coord.x, coord.z + 1 });
	nearbyChunks[CHUNK_BACK]  = getChunk({ coord.x, coord.z - 1 });
}

void ChunkManager::removeBlock(const LocationXYZ& loc) {
	Chunk* chunk = getChunk({ loc.x / CHUNK_SIZE, loc.z / CHUNK_SIZE });
	LocationXYZ blockLoc = getBlockLocation(loc);
	chunk->chunkData.set(blockLoc, BlockID::AIR);
}

void ChunkManager::placeBlock(const LocationXYZ& loc, BlockID blockID) {
	Chunk* chunk = getChunk({ loc.x / CHUNK_SIZE, loc.z / CHUNK_SIZE });
	LocationXYZ blockLoc = getBlockLocation(loc);
	chunk->chunkData.set(blockLoc, blockID);
}

Chunk* ChunkManager::getChunk(const ChunkXZ& coord) {
	if(!_chunkExists(coord)) {
		std::lock_guard<std::mutex> lock(chunkMutex);
		_chunks.emplace(coord, new Chunk(this, coord));
	}

	return _chunks[coord];
}
/*
Chunk* ChunkManager::createChunk(const ChunkXZ& coord, bool generateEverything) {
	Chunk* chunk = getChunk(coord);

	if(chunk == nullptr) {
		chunk = new Chunk(this, coord);

		std::lock_guard<std::mutex> lock(mutex);
		if(generateEverything) {
			{
				chunk->generateChunkData();
				chunk->generateChunkMesh();
			}
		}
		_chunks.emplace(coord, chunk);
	}

	return chunk;
}
*/
LocationXYZ ChunkManager::getBlockLocation(LocationXYZ loc) {
	LocationXYZ l{ loc.x % CHUNK_SIZE, loc.y % CHUNK_SIZE, loc.z % CHUNK_SIZE };
	if(loc.x < 0) l.x += CHUNK_SIZE;
	if(loc.y < 0) l.y += CHUNK_HEIGHT;
	if(loc.z < 0) l.z += CHUNK_SIZE;

	return l;
}

BlockID ChunkManager::getBlockID(const LocationXYZ& loc) {
	Chunk* chunk = getChunk({ loc.x / CHUNK_SIZE, loc.z / CHUNK_SIZE });
	return chunk->chunkData.get(getBlockLocation(loc));
}


bool ChunkManager::_chunkExists(const ChunkXZ& coord) {
	auto iter = _chunks.find(coord);
	return (iter != _chunks.end());
}

void ChunkManager::_generateChunks() {
	std::this_thread::sleep_for(std::chrono::microseconds(50));
	
	int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE);
	int currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);


	for(int i = 0; i < RENDER_DISTANCE; i++)
	for(int x = currentChunkX - i; x <= currentChunkX + i; x++)
	for(int z = currentChunkZ - i; z <= currentChunkZ + i; z++) {
		if(x <= 0 || z <= 0) continue;
		//if(x >= 4 || z >= 4) continue;

		Chunk* chunk = getChunk({ x, z });
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData();

		if(!chunk->meshesGenerated && chunk->chunkDataGenerated)
			chunk->generateChunkMesh();
	}
}
