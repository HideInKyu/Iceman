#include "Actor.h"
#include "StudentWorld.h"
// ----****----
#include <cmath>

Actor::~Actor() {}

Entity::~Entity() {}

Iceman::~Iceman() {}

void Iceman::update() {
	if (!isActive())
		return;
	handleInput();
}

void Iceman::interactWith(Actor* a) {
	if (!a->isActive())
		return;
	/*
	if (a->isGoodieObject()) {
		//TODO: PROBABLY A BETTER WAY TO DO THIS:
		else if (dynamic_cast<SonarKit*>(a) != nullptr) {
			incSonarCharges();
		}
	}
	*/
}

void Iceman::handleInput()
{
	int ch;
	if (getStudentWorld()->getKey(ch) == true)
	{
		int x = getX();
		int y = getY();
		const StudentWorld* sw = getStudentWorld();

		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getDirection() == Direction::left && sw->checkCoordsAreValid(x - 1, y, getHitBoxSize()))
				moveTo(x - 1, y);
			else
				setDirection(Direction::left);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == Direction::right && sw->checkCoordsAreValid(x + 1, y, getHitBoxSize()))
				moveTo(x + 1, y);
			else
				setDirection(Direction::right);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == Direction::up && sw->checkCoordsAreValid(x, y + 1, getHitBoxSize()))
				moveTo(x, y + 1);
			else
				setDirection(Direction::up);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == Direction::down && sw->checkCoordsAreValid(x, y - 1, getHitBoxSize()))
				moveTo(x, y - 1);
			else
				setDirection(Direction::down);
			break;
		case 'z':
		case 'Z':
			handleSonarKeyInput();
			break;
		case KEY_PRESS_TAB:
			handleGoldNuggetKeyInput();
			break;

		case KEY_PRESS_SPACE:
			handleSquirtKeyInput();
		}
	}
}

void Iceman::handleSonarKeyInput()
{
	if (getNumSonarCharges() > 0) {
		getStudentWorld()->playSound(SOUND_SONAR);
		getStudentWorld()->useSonarKit(this);
		decNumSonarCharges();
	}
}

void Iceman::handleGoldNuggetKeyInput()
{
	if (getNumGoldNuggets() > 0) {
		GoldNugget* g = new GoldNugget(getX(), getY(), getStudentWorld());
		g->setCanProtestorPickUp(true);
		g->setVisible(true);
		getStudentWorld()->addActor(g);
	}
}

void Iceman::handleSquirtKeyInput()
{
	if (getNumWaterSquirts() > 0) {
		// find coords to place squirt in front of Player:
		int x = getX();
		int y = getY();


		switch (getDirection()) {
		case GraphObject::Direction::down:
			y -= 4;
			break;
		case GraphObject::Direction::left:
			x -= 4;
			break;
		case GraphObject::Direction::up:
			y += 4;
			break;
		
		case GraphObject::Direction::right:
		default:
			x += 4;
			break;
		}
		
		Squirt* s = new Squirt(x, y, getDirection(), getStudentWorld());
		getStudentWorld()->addActor(s);
		decNumWaterSquirts();
	}
}

Environment::~Environment() {}

Goodies::~Goodies() {}

Boulder::~Boulder() {}
void Boulder::update() {
	if (!isActive())
		return;

	if (getState() == "stable") {}

	else if (getState() == "waiting") {
		if (getCurrentWaitingTick() > 0)
			decCurrentWaitingTick();
		else {
			setState("falling");
			setClippable(false);
			getStudentWorld()->playSound(SOUND_FALLING_ROCK);
		}
	}

	else if (getState() == "falling") {
		if (getY() < 0)
			setActive(false);
		else {
			moveTo(this->getX(), this->getY() - 1);
		}
	}

	else if (getState() == "crashed")
		setActive(false);
}

void Boulder::interactWith(Actor* a)
{
	if (a->isEntityObject()) {
		std::string state = getState();
		if (state == "falling") {
			static_cast<Entity*>(a)->takeDamage(10);
		}
	}
}

void Squirt::update()
{
	if (!isActive())
		return;

	if (getTravelDistance() >= MAX_TRAVEL_DISTANCE || !getStudentWorld()->isEmptySpace(this) ){
		setActive(false);
		return;
	}

	else if (getTravelDistance() < MAX_TRAVEL_DISTANCE) {
		incrementDistanceTraveled();
	}

	// handle direction:
	int x = getX();
	int y = getY();

	switch (getDirection()) {
	case GraphObject::Direction::down:
		moveTo(x, y - 1);
		break;
	case GraphObject::Direction::left:
		moveTo(x-1, y);
		break;
	case GraphObject::Direction::up:
		moveTo(x, y + 1);
		break;
	case GraphObject::Direction::right:
	default:
		moveTo(x + 1, y);
		break;
	}
}

GoldNugget::~GoldNugget() {}

void GoldNugget::update()
{
	if (!isActive())
		return;

	if (lifetimeTicks < 0)
		setActive(false);

	bool permanent = canPlayerPickUp;
	if (!permanent)
		lifetimeTicks--;
}

void GoldNugget::interactWith(Actor* a)
{
	if (a->isPlayerObject() && canPlayerPickUp) {
		Iceman* player = dynamic_cast<Iceman*>(a);
		if (player != nullptr) {
			player->incGoldNuggets();
			getStudentWorld()->increaseScore(25);
			getStudentWorld()->playSound(SOUND_GOT_GOODIE);
			setActive(false);
		}
	}
}

BarrelOfOil::~BarrelOfOil() {}

void BarrelOfOil::update()
{
	if (!isActive())
		return;
}

void BarrelOfOil::interactWith(Actor* a) {
	if (a->isPlayerObject()) {
		StudentWorld* playerWorld = a->getStudentWorld();
		setActive(false);
		playerWorld->playSound(SOUND_FOUND_OIL);
		playerWorld->increaseScore(1000);
		playerWorld->decNumBarrelsOfOil();
	}
}

Ice::~Ice() {}

void SonarKit::update()
{
	if (!isActive())
		return;

	if (lifetimeTicks <= 0)
		setActive(false);

	lifetimeTicks--;
}

void SonarKit::interactWith(Actor* a) {
	if (a->isPlayerObject()) {
		Iceman* player = dynamic_cast<Iceman*>(a);
		if (player != nullptr) {
			player->incSonarCharges();
			getStudentWorld()->increaseScore(75);
			getStudentWorld()->playSound(SOUND_GOT_GOODIE);
			setActive(false);
		}
	}
}

void WaterPool::update()
{
	if (!isActive())
		return;

	if (lifetimeTicks <= 0)
		setActive(false);

	lifetimeTicks--;
}

void WaterPool::interactWith(Actor* a)
{
	if (a->isPlayerObject()) {
		Iceman* player = dynamic_cast<Iceman*>(a);
		if (player != nullptr) {
			setActive(false);
			getStudentWorld()->playSound(SOUND_GOT_GOODIE);
			for (int i = 1; i <= 5; i++)
				player->incWaterSquirts();
			getStudentWorld()->increaseScore(100);
		}

	}
}
