#ifndef ENTITY_H
#define ENTITY_H

class AABB;

class Entity {

protected:
	AABB* boundingBox;


public:
	Entity() { }
	~Entity() { }


};

#endif // ENTITY_H