#include "Shader.h"
#include "Log.h"
#include "ChunkMesh.h"
#include "TextureAtlas.h"
#include "Game.h"
#include "Player.h"
#include "Constants.h"


Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) {
	_vertexShader = _load(GL_VERTEX_SHADER, vertexShader);
	_fragmentShader = _load(GL_FRAGMENT_SHADER, fragmentShader);

	if(geometryShader != "")
		_geometryShader = _load(GL_GEOMETRY_SHADER, geometryShader);

	_link();
}

Shader::~Shader() {
	glDeleteProgram(_program);
}

void Shader::bind() { 
	glUseProgram(_program);
}

void Shader::unbind() {
	glUseProgram(0);
}


void Shader::setBool(const std::string& name, bool value) {
	bind();
	glUniform1i(glGetUniformLocation(_program, name.c_str()), (int) value);
}

void Shader::setInt(const std::string& name, int value) {
	bind();
	glUniform1i(glGetUniformLocation(_program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	bind();
	glUniform1f(glGetUniformLocation(_program, name.c_str()), value);
}



void Shader::setVec2(const std::string& name, const glm::vec2& value) {
	bind();
	glUniform2fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) {
	bind();
	glUniform2f(glGetUniformLocation(_program, name.c_str()), x, y);
}



void Shader::setVec3(const std::string& name, const glm::vec3& value) {
	bind();
	glUniform3fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) {
	bind();
	glUniform3f(glGetUniformLocation(_program, name.c_str()), x, y, z);
}



void Shader::setVec4(const std::string& name, const glm::vec4& value) {
	bind();
	glUniform4fv(glGetUniformLocation(_program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
	bind();
	glUniform4f(glGetUniformLocation(_program, name.c_str()), x, y, z, w);
}



void Shader::setMat2(const std::string& name, const glm::mat2& mat) {
	bind();
	glUniformMatrix2fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) {
	bind();
	glUniformMatrix3fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
	bind();
	glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLuint Shader::_load(GLenum type, const std::string& shaderFile) {
	GLuint shader = glCreateShader(type);

	std::string code = _readFile(shaderFile);
	const char* c = code.c_str();
	glShaderSource(shader, 1, &c, NULL);

	GLint success;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLint infoLogSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);

		GLchar* infoLog = new GLchar[infoLogSize];
		glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog);
		Log::write(Log::INFO, infoLog);
		delete[] infoLog;
	}

	return shader;
}

void Shader::_link() {
	_program = glCreateProgram();

	if(_vertexShader != 0)
		glAttachShader(_program, _vertexShader);
	if(_fragmentShader != 0)
		glAttachShader(_program, _fragmentShader);
	if(_geometryShader != 0)
		glAttachShader(_program, _geometryShader);

	GLint success;
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if(!success) {
		GLint infoLogSize;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogSize);
		GLchar* infoLog = new GLchar[infoLogSize];
		glGetProgramInfoLog(_program, infoLogSize, nullptr, infoLog);

		fprintf(stderr, "Linking error in shader: %s\n", infoLog);
		delete[] infoLog;
	}

	glDetachShader(_program, _vertexShader);
	glDetachShader(_program, _fragmentShader);
	glDetachShader(_program, _geometryShader);

	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);
	glDeleteShader(_geometryShader);
}

std::string Shader::_readFile(const std::string path) {
	std::ifstream file(PATH_SHADERS + path);
	std::string code = "";

	if(!file)
		throw std::runtime_error("Failed to read shader file!");

	std::string line = "";
	while(std::getline(file, line))
		code += line + "\n";

	file.close();
	return code;
}

GLuint Shader::_getCompiledShader(GLuint shader, const std::string& source) {
	GLuint shader_program = glCreateShader(shader);

	const char* c_source = source.c_str();
	glShaderSource(shader_program, 1, &c_source, nullptr);
	glCompileShader(shader_program);

	GLint compiled;
	glGetShaderiv(shader_program, GL_COMPILE_STATUS, &compiled);
	if(!compiled) {
		int len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		GLchar* infoLog = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, infoLog);

		fprintf(stderr, "Compilation error in shader: %s\n", infoLog);
		delete[] infoLog;
	}

	return shader;
}
