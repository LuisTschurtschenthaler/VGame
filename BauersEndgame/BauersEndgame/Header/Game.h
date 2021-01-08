#ifndef GAME_H
#define GAME_H

#include <string>

class Shop;

class Game {

private:
	Shop* _shop;


public:
	bool stop = false;


public:
	Game();
	~Game();

	void init();
	void update();
	//void loadFrame(const std::string& path);

};

#endif // GAME_H
