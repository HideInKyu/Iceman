#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
// ---****---
#include "StudentWorld.h"
class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		:GraphObject(imageID, startX, startY, direction, size, depth ), hitBoxSize(size * 4), studentWorld(sw) {}
	
	virtual ~Actor() = 0;
	virtual void update() = 0;
	virtual void doSomething() = 0;
	// TODO: ADD ANNOYED FUNCTION AND VARIABLE
	
	// Getters 
	StudentWorld* getStudentWorld() const { return studentWorld; }
	bool isActive() const{ return active; }
	bool isClippable() const { return clippable; }
	int getHitBoxSize() const { return hitBoxSize; }
	virtual bool hasGravity() const { return false; }

	// Setters/Mutators
	void toggleActive() { active = !active; }
	void toggleClippable() { clippable = !clippable; }


private:
	StudentWorld* studentWorld;
	GraphObject::Direction direction = GraphObject::Direction::right;
	bool active = true;
	bool clippable = false;
	int hitBoxSize; // **NOTE**: it seems hitBoxSize 4 times double the inherited: GraphObject::size
};

class Environment : public Actor {
public:
	Environment(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		:Actor(imageID, startX, startY, direction, size, depth, sw) {}
	~Environment();
	void update();
	void doSomething();
private:
};

class Goodies : public Environment {
public:
	Goodies(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		:Environment(imageID, startX, startY, direction, size, depth, sw) {}
	~Goodies();
	virtual void update();
	virtual void doSomething();
private:
};

class Ice : public Environment {
public: 
	Ice(int startX, int startY, StudentWorld* sw)
		:Environment(IID_ICE, startX, startY, Direction::right, 0.25, 3, sw) {
	}
	~Ice();
	void update();
	void doSomething();
private:
};

class Boulder : public Environment {
public: 
	Boulder(int startX, int startY, StudentWorld* sw)
		:Environment(IID_BOULDER, startX, startY, Direction::right, 1, 1, sw) {}
	~Boulder();
	void update();
	void doSomething();
private:
	bool waiting = true;
	int waitingTicks = 30;
	bool hasGravity() const override { return true; }
};

class GoldNugget : public Goodies {
public:
	GoldNugget(int startX, int startY, StudentWorld* sw)
		:Goodies(IID_GOLD, startX, startY, Direction::right, 1.00, 0, sw) {}
	~GoldNugget();
	void update();
	void doSomething();
private:
};

class BarrelOfOil : public Goodies {
public:
	BarrelOfOil(int startX, int startY, StudentWorld* sw)
		:Goodies(IID_BARREL, startX, startY, Direction::right, 1.00, 0, sw) {}
	~BarrelOfOil();
	void update();
	void doSomething();
private:
};


class Iceman : public Actor {
public:
	Iceman(int startX, int startY, StudentWorld* sw) : Actor(IID_PLAYER, startX, startY, Direction::right, 1.00, 0, sw) {}
	~Iceman();
	void update();
	void doSomething();
private:
	int hitPoints = 10;
	int waterSquirts = 5;
	int sonarCharges = 1;
	int goldNuggets = 0;

	// update() helpers:
	void handleInput();
};

//---****CODE FOR ROY****---
// TODO: Created derived classes HardcoreProtestor and RegularProtestor
// Please read the Object Orientated Programming Tips on Project 4 Page
// I haven't read the docs for Protestors yet but you can probably start implementing their search algorithms
// It's probably best for the algorithm to be implemented in StudentWorld since it has access to Ice2DArray
// >>> Have the alogrithm move the protestors in StudentWorld 
class Protestor : public Actor {
public:
	Protestor(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		: Actor(imageID, startX, startY, direction, size, depth, sw) {}
	~Protestor() = 0;
};

#endif ACTOR_H_