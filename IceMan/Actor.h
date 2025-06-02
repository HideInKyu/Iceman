#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// ----****----
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

//Forward Declarations:
class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, GraphObject::Direction, double size, unsigned int depth, StudentWorld* sw)
		:GraphObject(imageID, startX, startY, direction, size, depth), hitBoxSize(size * 4), studentWorld(sw) {}
	virtual ~Actor() = 0;

	// getters:
	StudentWorld* getStudentWorld() const { return studentWorld; }
	bool isActive() const { return active; }
	bool isClippable() const { return clippable; }
	double getHitBoxSize() const { return hitBoxSize; }
	std::string getState() const { return state; }

	// setters/mutators:
	void toggleActive() { active = !active; }
	void setActive(bool shouldIBeActive) { active = shouldIBeActive; }
	virtual bool setState(std::string newState) {
		state = newState;
		return true;
	}


	// Class Groups by Behaviors:
	virtual bool hasGravity() const { return false; }
	virtual bool isEntityObject() const { return false; }
	virtual bool isEnvironmentObject() const { return false; }
	virtual bool isPlayerObject() const { return false; }
	virtual bool isGoodieObject() const { return false; }

	// Functionality:
	virtual void update() {}
	virtual void interactWith(Actor* a) {}

	struct HitBox {
		int x, y; //bottom-left corner
		int size;
	};
protected:
	void setClippable(bool shouldIBeClippable) { clippable = shouldIBeClippable; }

private:
	StudentWorld* studentWorld;
	GraphObject::Direction direction = GraphObject::Direction::right;
	bool active = true;
	bool clippable = false;
	int hitBoxSize = 0;
	std::string state = "";
};

class Entity : public Actor {
public:
	Entity(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw, int hp)
		: Actor(imageID, startX, startY, direction, size, depth, sw) {
		hitPoints = hp;
		setClippable(true);
	}
	~Entity();

	// identifiers:
	bool isEntityObject() const override { return true; }


	int getHitPoints() const { return hitPoints; }
	void takeDamage(unsigned int damageTaken) { hitPoints -= damageTaken; }

private:
	int hitPoints = 0;
};

class Iceman : public Entity {
public:
	Iceman(int startX, int startY, StudentWorld* sw) : Entity(IID_PLAYER, startX, startY, Direction::right, 1.00, 0, sw, 10) { }
	~Iceman();
	void update();

	// getters
	int getNumWaterSquirts() const { return waterSquirts; }
	int getNumSonarCharges() const { return sonarCharges; }
	int getNumGoldNuggets() const { return goldNuggets; }

	// setters/mutators
	void incWaterSquirts() { waterSquirts++; }
	void incSonarCharges() { sonarCharges++; }
	void incGoldNuggets() { goldNuggets++; }
	void decNumWaterSquirts() { waterSquirts--; }
	void decNumSonarCharges() { sonarCharges--; }
	void decNumGoldNuggets() { goldNuggets--; }

	// Identifiers:
	bool isPlayerObject() const override { return true; }

	// Functionality:
	void interactWith(Actor* a) override;

private:
	int waterSquirts = 5;
	int sonarCharges = 1;
	int goldNuggets = 0;

	// update() helpers:
	void handleInput();
	void handleSonarKeyInput();
	void handleGoldNuggetKeyInput();

	// Abilities:
	// DECLARE: void dropGoldNugget();
};

class Environment : public Actor {
public:
	Environment(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		:Actor(imageID, startX, startY, direction, size, depth, sw) {}

	~Environment();
	// DECLARE: void update();

	// identifiers:
	virtual bool isEnvironmentObject() const override { return true; }
private:
};

class Goodies : public Environment {
public:
	Goodies(int imageID, int startX, int startY, GraphObject::Direction direction, double size, unsigned int depth, StudentWorld* sw)
		:Environment(imageID, startX, startY, direction, size, depth, sw) {}
	~Goodies();

	bool isGoodieObject() const override { return true; }
	// DECLARE: virtual void update();
private:
};

class Boulder : public Environment {
public:
	Boulder(int startX, int startY, StudentWorld* sw)
		:Environment(IID_BOULDER, startX, startY, Direction::right, 1, 1, sw) {
		setClippable(true);
	}
	~Boulder();
	bool hasGravity() const { return true; }
	void update() override;

	// getters
	int getCurrentWaitingTick() const { return currentWaitingTick; }

	// settters/mutators
	void decCurrentWaitingTick() { currentWaitingTick--; }

	// Functionality:
	void interactWith(Actor* a) override;
private:
	int DEFAULT_BOULDER_WAITING_TICKS = 30;
	int currentWaitingTick = DEFAULT_BOULDER_WAITING_TICKS;
};

class GoldNugget : public Goodies {
public:
	GoldNugget(int startX, int startY, StudentWorld* sw)
		:Goodies(IID_GOLD, startX, startY, Direction::right, 1.00, 2, sw) {
	}
	~GoldNugget();


	// getters
	bool getCanProtestorPickUp() const { return canProtestorPickUp; }
	bool getCanPlayerPickUp() const { return canPlayerPickUp; }

	// setters/mutators
	void setCanProtestorPickUp(bool shouldProtestorPickUp) {
		canProtestorPickUp = shouldProtestorPickUp;
		canPlayerPickUp = !canProtestorPickUp;
		isPermanent = canPlayerPickUp;
	}
	void setCanPlayerPickUp(bool shouldPlayerPickUp) {
		canPlayerPickUp = shouldPlayerPickUp;
		canProtestorPickUp = !canPlayerPickUp;
		isPermanent = canPlayerPickUp;
	}

	// Functionality:
	void update();
	void interactWith(Actor* a) override;
private:
	// SET LIFETIME_TICKS IN CONSTR BASED ON SOMETHING IDK
	int lifetimeTicks = 120;

	// INVARIANT: isPermanent = canPlayerPickUp = !canProtestorPickUp
	bool canPlayerPickUp = true;
	bool canProtestorPickUp = false;
	bool isPermanent = canPlayerPickUp;
};

class BarrelOfOil : public Goodies {
public:
	BarrelOfOil(int startX, int startY, StudentWorld* sw)
		:Goodies(IID_BARREL, startX, startY, Direction::right, 1.00, 2, sw) {
	}
	~BarrelOfOil();

	void update();
	void interactWith(Actor* a) override;
private:
};

class SonarKit : public Goodies {
public:
	SonarKit(StudentWorld* sw, int level)
		:Goodies(IID_SONAR, 0, 60, Direction::right, 1.00, 2, sw) {
		int n = 10 * level;
		lifetimeTicks = std::max(100, 300 - n);
	}

	void update() override;
	void interactWith(Actor* a) override;
private:
	int lifetimeTicks = 0;
};

class Ice : public Environment {
public:
	Ice(int startX, int startY, StudentWorld* sw)
		:Environment(IID_ICE, startX, startY, Direction::right, 0.25, 3, sw) {
		setClippable(true);
	}
	~Ice();
	// DECLARE: void update();
private:
};



#endif // ACTOR_H_
