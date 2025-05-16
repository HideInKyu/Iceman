#include "Actor.h"
#include "StudentWorld.h"
// ---****---
using namespace std;

// Actor Implementation
Actor::~Actor() {} // Worst Case Scenario: no Actor-derived class destructor available
void Actor::update() {}

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
	if (!waiting) {
		if (waitingTicks > 0)
			waitingTicks--;
		else {

			moveTo(this->getX(), this->getY() - 1);
		}
	}

} 

// Goodies Implementation:
Goodies::~Goodies() {}
void Goodies::update() {}

// GoldNugget Implementation:
GoldNugget::~GoldNugget() {}
void GoldNugget::update() {}

// BarrelOfOil Implementation:
BarrelOfOil::~BarrelOfOil(){}
void BarrelOfOil::update() {}


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
	if (studentWorld->getKey(ch) == true)
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
			studentWorld->decLives();
		}
	}
}
//Protestor Implementation
