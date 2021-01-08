#ifndef SKY_H
#define SKY_H

class Shader;

class Sky {

private:
	Shader* _shader;


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
