#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <GLEW/GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class Shader {

private:
	GLuint _program;
	GLuint _vertexShader, _fragmentShader, _geometryShader;


public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = "");
	~Shader();
	
	void bind();
	void unbind();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w);
	
	void setMat2(const std::string& name, const glm::mat2& value) const;
	void setMat3(const std::string& name, const glm::mat3& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;

	GLuint getShaderProgram() const { return _program; }

private:
	GLuint _load(GLenum type, const std::string& shaderFile);
	void _link();
	std::string _readFile(const std::string file);
	GLuint _getCompiledShader(GLuint shader, const std::string& source);

};

#endif // SHADER_H
