#ifndef SKY_H
#define SKY_H

#include <GLM/glm.hpp>
#include <vector>
class Shader;
class Camera;

class Sky {

private:
	Shader* _shader;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int _VBO;
	unsigned int _IBO;


public:
	Sky();
	~Sky();

	void init();
	void draw(Camera& camera);

private:
	float _calculateStarIntensity(float dayTime);

};

#endif // SKY_H
