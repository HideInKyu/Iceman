#include "Actor.h"
#include "StudentWorld.h"
// ---****---
#include "GameController.h"
using namespace std;

// Actor Implementation
Actor::~Actor() {} // Worst Case Scenario: no Actor-derived class destructor available
void Actor::update() {}
void Actor::handlePlayerInteraction(Iceman* player) {}
// TODO: implement collidesWith(...)


// Environment Implementation
Environment::~Environment() {}
void Environment::update() {}

// Ice Implementation:
Ice::~Ice() {}
void Ice:: update() {} // TODO: Implement Ice::update()


// Boulder Implementation:
Boulder::~Boulder(){}
void Boulder::update(){
	// TODO: Implement Boulder::update()
	if (isWaiting()) {
		int currentWaitingTick = getCurrentWaitingTick();
		if (currentWaitingTick > 0)
			setWaitingTicks(currentWaitingTick-1);
		else {
			moveTo(this->getX(), this->getY() - 1);
		}
	}
} 
// Goodies Implementation:
Goodies::~Goodies() {}
void Goodies::update() {

}

// GoldNugget Implementation:
GoldNugget::~GoldNugget() {}
void GoldNugget::update() {
	if (!isPermanent) {
		lifetimeTicks--;
		if (lifetimeTicks <= 0) {
			toggleActive();
		}
	}
}
void GoldNugget::handlePlayerInteraction(Iceman* player) {
	if (!isActive())
		return;
	
	if (canPlayerPickUp) {
		// TODO: ADD SOUND EFFECT
		player->incGoldNuggets();
		getStudentWorld()->increaseScore(10);
		toggleActive();
	}
}

// BarrelOfOil Implementation:
BarrelOfOil::~BarrelOfOil(){}
void BarrelOfOil::update() {
}
void BarrelOfOil::handlePlayerInteraction(Iceman* player) {
	if (!isActive())
		return;

	//TODO: MAKE WORK! GameWorld::playSound(SOUND_FOUND_OIL);
	getStudentWorld()->increaseScore(1000);

	toggleActive();
}


// Iceman Implementation:
Iceman::~Iceman(){}
void Iceman::update() {
	if (!isActive())
		return;
	handleInput();
}

void Iceman::handleInput()
{
	int ch;
	if (getStudentWorld()->getKey(ch) == true)
	{
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getDirection() == Direction::left)
				moveTo(getX() - 1, getY());
			else
				setDirection(Direction::left);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == Direction::right)
				moveTo(getX() + 1, getY());
			else
				setDirection(Direction::right);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == Direction::up)
				moveTo(getX(), getY() + 1);
			else
				setDirection(Direction::up);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == Direction::down)
				moveTo(getX(), getY() - 1);
			else
				setDirection(Direction::down);
			break;
		case KEY_PRESS_SPACE:
			dropGoldNugget();
		}
	}
}

void Iceman::dropGoldNugget() {
	if (goldNuggets > 0) {
		getStudentWorld()->handlePlayerDropGoldNugget();
		goldNuggets--;
	}
}

// TODO: IMPLEMENT METHOD
void SonarKit::handlePlayerInteraction(Iceman* player)
{
}

//Protestor Implementation

