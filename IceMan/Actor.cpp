#include "Actor.h"
#include "StudentWorld.h"
// ---****---
using namespace std;

// Actor Implementation
Actor::~Actor() {} // Worst Case Scenario: no Actor-derived class destructor available
void Actor::update() {}
void Actor::doSomething() {}
// TODO: implement collidesWith(...)


// Environment Implementation
Environment::~Environment() {}
void Environment::update() {}
void Environment::doSomething() {}

// Ice Implementation:
Ice::~Ice() {}
void Ice:: update() {} // TODO: Implement Ice::update()
void Ice::doSomething() {}


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
void Boulder::doSomething() {}

// Goodies Implementation:
Goodies::~Goodies() {}
void Goodies::update() {

}
void Goodies::doSomething() { 
	if (!isActive())
		return;
	toggleActive();
}

// GoldNugget Implementation:
GoldNugget::~GoldNugget() {}
void GoldNugget::update() {}
void GoldNugget::doSomething() { 
	if (!isActive())
		return;
	toggleActive();
}

// BarrelOfOil Implementation:
BarrelOfOil::~BarrelOfOil(){}
void BarrelOfOil::update() {}
void BarrelOfOil::doSomething() { 
	if (!isActive())
		return;
	toggleActive(); 
}


// Iceman Implementation:
Iceman::~Iceman(){}
void Iceman::update() {
	if (!isActive())
		return;
	handleInput();
}
void Iceman::doSomething() {}
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
			getStudentWorld()->decLives();
		}
	}
}
//Protestor Implementation
