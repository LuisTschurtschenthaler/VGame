#ifndef SKY_H
#define SKY_H

#include <GLM/glm.hpp>
#include <vector>
class Shader;
class Player;

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
	void draw(Player* player);

	glm::vec3 getSunPosition();

private:
	float _calculateStarIntensity(float dayTime);

};

#endif // SKY_H
