#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
class Shader;


class Skybox {

private:
	static float skyboxVertices[];

	Shader* _skyboxShader;
	unsigned int _VAO, _VBO;


public:
	Skybox();
	~Skybox();

	void update();
	void draw();

};

#endif // SKYBOX_H
