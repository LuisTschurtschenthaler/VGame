#ifndef MESH_H
#define MESH_H

#include <string>
#include <unordered_map>
#include <GLM/glm.hpp>
#include "Vertex.h"

class MeshData {

private:
	unsigned int _vbo;
	unsigned int _ibo;
	unsigned int _ebo;
	int _size;

public:
	MeshData(int indexSize);
	virtual ~MeshData();

	inline unsigned int getVBO() { return _vbo; }
	inline unsigned int getIBO() { return _ibo; }
	inline unsigned int getEBO() { return _ebo; }
	inline int getSize() { return _size; }

};

class Mesh {

private:
	static std::unordered_map<std::string, MeshData*> _meshes;

	std::string _file;
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	glm::mat4 _model;
	MeshData* _meshData;


public:
	Mesh(const std::string& file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Mesh() { }

	void draw() const;
	void move(const glm::vec3 position);
	void rotate(const glm::vec3 rotation);
	void scale(const glm::vec3 scale);

	void update();
	const glm::mat4& getModel() { return _model; };
	const glm::vec3& getPosition() { return _position; }

private:
	void _initMesh(Vertex* vertices, int vertexSize, int* indices, int indexSize, bool calculateNormals);
	void _calculateNormals(Vertex* vertices, int vertexSize, int* indices, int indexSize);

};

#endif // MESH_H