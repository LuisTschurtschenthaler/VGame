#include "Mesh.h"
#include "Log.h"
#include <GLEW/GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <iostream>
#include <cassert>

std::unordered_map<std::string, MeshData*> Mesh::_meshes;

MeshData::MeshData(int indexSize) {
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	glGenBuffers(1, &_ebo);
	_size = indexSize;
}

MeshData::~MeshData() {
	if(_vbo)
		glDeleteBuffers(1, &_vbo);

	if(_ibo)
		glDeleteBuffers(1, &_ibo);
}


Mesh::Mesh(const std::string& file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: _file(file), _position(position), _rotation(rotation), _scale(scale),
	_model(glm::mat4(1.f)), _meshData(nullptr) {

	std::unordered_map<std::string, MeshData*>::const_iterator it = _meshes.find(file);
	if(it == _meshes.end()) {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
		if(!scene)
			Log::write(Log::ERROR, "Failed to load mesh: " + file);

		const aiMesh* model = scene->mMeshes[0];

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		const aiVector3D aiZeroVector(0.f);
		for(unsigned int i = 0; i < model->mNumVertices; i++) {
			const aiVector3D* position = &(model->mVertices[i]);
			const aiVector3D* normal = &(model->mNormals[i]);
			//const aiVector3D* texCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;

			Vertex vert(glm::vec3(position->x, position->y, position->z),
						glm::vec3(normal->x, normal->y, normal->z));

			vertices.push_back(vert);
		}

		for(unsigned int i = 0; i < model->mNumFaces; i++) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		_initMesh(&vertices[0], vertices.size(), (int*) &indices[0], indices.size(), true);
		_meshes.insert(std::pair<std::string, MeshData*>(file, _meshData));

	} else {
		_meshData = it->second;
	}
}

void Mesh::draw() const {
	glBindBuffer(GL_ARRAY_BUFFER, _meshData->getVBO());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position)); 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshData->getIBO());
	glDrawElements(GL_TRIANGLES, _meshData->getSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Mesh::_initMesh(Vertex* vertices, int vertexSize, int* indices, int indexSize, bool calculateNormals) {
	_meshData = new MeshData(indexSize);

	if(calculateNormals)
		_calculateNormals(vertices, vertexSize, indices, indexSize);

	glBindBuffer(GL_ARRAY_BUFFER, _meshData->getVBO());
	glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshData->getIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);
}

void Mesh::move(const glm::vec3 position) {
	_position += position;
}

void Mesh::rotate(const glm::vec3 rotation) {
	_rotation += rotation;
}

void Mesh::scale(const glm::vec3 scale) {
	_scale += scale;
}

void Mesh::update() {
	_model = glm::mat4(1.f);
	_model = glm::translate(_model, _position);
	_model = glm::rotate(_model, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	_model = glm::rotate(_model, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	_model = glm::rotate(_model, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	_model = glm::scale(_model, glm::vec3(_scale));
}

void Mesh::_calculateNormals(Vertex* vertices, int vertexSize, int* indices, int indexSize) {
	for(int i = 0; i < indexSize; i += 3) {
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = vertices[i1].position - vertices[i0].position;
		glm::vec3 v2 = vertices[i2].position - vertices[i0].position;

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for(int i = 0; i < vertexSize; i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);
}