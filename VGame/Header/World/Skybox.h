#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
class Shader;


class Skybox {

private:
	static std::vector<float> _vertices;
	static std::vector<unsigned int> _indices;

	Shader* _skyboxShader;
	unsigned int _VBO, _IBO;
	bool _isBuffered;


public:
	Skybox();
	~Skybox();

	void update();
	void draw();

};

#endif // SKYBOX_H
