// DESIGN PRINCIPLE: StudentWorld handles/Signals to Actor classes that they are SUPPOSED to interact with one another.

#include "StudentWorld.h"
#include <string>
using namespace std;
//---****---
#include <vector>
#include "Actor.h"

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld()
{
	// Delete ice2DArray
	for (int col = 0; col < ICE_ARRAY_SIZE; col++) {
		for (int row = 0; row < ICE_ARRAY_SIZE; row++) {
			delete ice2DArray[col][row];
		}
	}
	// Delete all Actors
	for (int i = 0; i < actors.size(); i++)
		delete actors[i];
	// Delete Player/Iceman
	delete iceman;
}

int StudentWorld::init() {

	// Initialize ice
	for (int col = 0; col < ICE_ARRAY_SIZE; col++) {
		for (int row = 0; row < ICE_ARRAY_SIZE; row++) {
			ice2DArray[col][row] = new Ice(col, row, this);
			ice2DArray[col][row]->setVisible(true);
		}
	}
	// Create Tunnel by deactivating Ice 
	for (int col = 30; col <= 33; col++) {
		for (int row = 4; row <= 59; row++) {
			//ice2DArray[col][row]->toggleActive();
			delete ice2DArray[col][row];
			ice2DArray[col][row] = nullptr;
		}
	}

	// Set Environment amounts:
	int currentLevel = int(getLevel());
	numBoulders = min(currentLevel / 2 + 2, 9);
	numGoldNuggets = max(5 - currentLevel / 2, 2);
	numBarrelsOfOil = min(2 + currentLevel, 21);

	// Distribute Environment Actors
	distributeOilFieldContents(actors);

	// Spawn Player:
	iceman = new Iceman(30, 60, this);
	iceman->setVisible(true);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	updateDisplayText();
	iceman->update();
	mineIce();
	handleCollisions(actors);
	updateAllActors();
	spawnRandomGoodies();
	removeDeadGameObjects();

	return determineGameStatus();
}

void StudentWorld::cleanUp()
{
	// Clean-up Ice Array
	for (int col = 0; col < ICE_ARRAY_SIZE; col++)
		for (int row = 0; row < ICE_ARRAY_SIZE; row++)
			delete ice2DArray[col][row];

	// Clean-up Actors
	for (int i = 0; i < actors.size(); i++) {
		delete actors[i];
		actors[i] = nullptr;
	}
	actors.clear();

	delete iceman;
}

// Private Functions:

// init() helpers:
void StudentWorld::distributeOilFieldContents(std::vector<Actor*>& actors)
{
	distributeBoulders(actors);
	distributeGoldNuggets(actors);
	distributeBarrelsOfOil(actors);
}

void StudentWorld::distributeBoulders(vector<Actor*>& actors)
{
	int boulderSpawnX = 0;
	int boulderSpawnY = 0;
	for (int i = 0; i < numBoulders; i++) {
		do {
			boulderSpawnX = static_cast<int>(rand() % (61 - 4) + 0);
			boulderSpawnY = static_cast<int>(rand() % 37 + 20);
		} while (isSpawnTooCloseToOtherActors(boulderSpawnX, boulderSpawnY, 1.0, actors) || isEmptySpace(boulderSpawnX, boulderSpawnY, 1.0));

		// Clear 4x4 Ice Around Boulders
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				delete ice2DArray[boulderSpawnX + j][boulderSpawnY + k];
				ice2DArray[boulderSpawnX + j][boulderSpawnY + k] = nullptr;
			}
		}
		Actor* temp = new Boulder(boulderSpawnX, boulderSpawnY, this);
		temp->setVisible(true);
		actors.push_back(temp);
	}

}

void StudentWorld::distributeGoldNuggets(std::vector<Actor*>& actors)
{
	int goldNuggetSpawnX = 0;
	int goldNuggetSpawnY = 0;

	for (int i = 0; i < numGoldNuggets; i++) {
		do {
			goldNuggetSpawnX = static_cast<int>(rand() % (61 - 4) + 0);
			goldNuggetSpawnY = static_cast<int>(rand() % 56 + 0);
		} while (isSpawnTooCloseToOtherActors(goldNuggetSpawnX, goldNuggetSpawnY, 1.0, actors) || isEmptySpace(goldNuggetSpawnX, goldNuggetSpawnY, 1.0));

		actors.push_back(new GoldNugget(goldNuggetSpawnX, goldNuggetSpawnY, this));
	}
}

void StudentWorld::distributeBarrelsOfOil(std::vector<Actor*>& actors)
{
	int barrelOfOilSpawnX = 0;
	int barrelOfOilSpawnY = 0;

	for (int i = 0; i < numBarrelsOfOil; i++) {
		do {
			barrelOfOilSpawnX = static_cast<int>(rand() % (61 - 4) + 0);
			barrelOfOilSpawnY = static_cast<int>(rand() % 56 + 0);
		} while (isSpawnTooCloseToOtherActors(barrelOfOilSpawnX, barrelOfOilSpawnY, 1.0, actors) || isEmptySpace(barrelOfOilSpawnX, barrelOfOilSpawnY, 1.0));

		actors.push_back(new BarrelOfOil(barrelOfOilSpawnX, barrelOfOilSpawnY, this));
	}
}

bool StudentWorld::isSpawnTooCloseToOtherActors(int x, int y, double size, const std::vector<Actor*>& actors)
{
	for (int i = 0; i < actors.size(); i++) {
		if (actors[i] == nullptr)
			continue;

		double distance = calculateDistance(x, y, size, actors[i]);
		if (distance < 6)
			return true;
	}

	return false;
}

bool StudentWorld::isEmptySpace(int x, int y, double size)
{
	int hitBoxSize = static_cast<int>(size * 4);
	// Counts space ABOVE icefield as empty space:
	// x: [0, 60)
	// y: [60, 64)
	if (y >= 60 && y < 64 && x >= 0 && x < 60)
		return true;

	// Out of bounds for ice2DArray
	if (x < 0|| y < 0 || x + hitBoxSize > ICE_ARRAY_SIZE || y + hitBoxSize > ICE_ARRAY_SIZE)
		return false; 

	for (int i = 0; i < hitBoxSize; i++) {
		for (int j = 0; j < hitBoxSize; j++) {
			if (ice2DArray[x + i][y + j] != nullptr)
				return false;
		}
	}

	return true;
}

bool StudentWorld::isEmptySpace(Actor* a)
{
	return isEmptySpace(a->getX(), a->getY(), a->getSize());
}

bool StudentWorld::checkCoordsAreValid(int x, int y, int size) const {
	if (x < 0 || y < 0 || x > ICE_ARRAY_SIZE - size || y > ICE_ARRAY_SIZE)
		return false;

	for (int i = 0; i < actors.size(); i++)
		if (actors[i] != nullptr
			&& actors[i]->isClippable()
			&& willActorsCollide(x, y, size, actors[i]))
			return false;

	return true;
}
void StudentWorld::updateDisplayText()
{
	string msg = "";
	msg += "Lvl: " + std::to_string(getLevel());
	msg += " Lives : " + std::to_string(getLives());
	msg += " Hlth : " + std::to_string(iceman->getHitPoints() * 10) + "%";
	msg += " Wtr: " + std::to_string(iceman->getNumWaterSquirts());
	msg += " Gld: " + std::to_string(iceman->getNumGoldNuggets());
	msg += " Oil Left : " + std::to_string(getNumBarrlesOfOil());
	msg += " Sonar: " + std::to_string(iceman->getNumSonarCharges());
	msg += " Scr: " + std::to_string(getScore());
	setGameStatText(msg);

}

// move() helpers:
void StudentWorld::removeDeadGameObjects()
{
	// Delete Ice objects from ice2DArray
	for (int i = 0; i < ICE_ARRAY_SIZE; i++)
		for (int j = 0; j < ICE_ARRAY_SIZE; j++)
			if (ice2DArray[i][j] != nullptr && !ice2DArray[i][j]->isActive()) {
				delete ice2DArray[i][j];
				ice2DArray[i][j] = nullptr;
			}

	// Delete all other inactive Actors
	for (int i = 0; i < actors.size(); i++)
		if (actors[i] != nullptr && !actors[i]->isActive()) {
			delete actors[i];
			actors[i] = nullptr;
		}
}

void StudentWorld::spawnRandomGoodies()
{
	int G1 = getLevel() * 30 + 290;
	if (rand() % (G1 + 1) == 0) {
		// TODO:
		// Spawn 1/5 Sonar Kit 4/5 Water Goodie:
		int G2 = (rand() % 5) + 1;
		if (G2 <= 4) {
			// Spawn Water
			int x = 0;
			int y = 0;
			do {
				x = rand() % (ICE_ARRAY_SIZE + 1);
				y = rand() % (ICE_ARRAY_SIZE + 1);
			} while (!isEmptySpace(x, y, 1.0));
			
			Actor* temp = new WaterPool(x,y, this, getLevel());
			actors.push_back(temp);
			temp = nullptr;
		}
		else if (G2 == 5) {
			// Spawn Sonar Kit
			Actor* temp = new SonarKit(this, getLevel());
			actors.push_back(temp);
			temp = nullptr;
		}
	}
}

void StudentWorld::handleCollisions(std::vector<Actor*>& actors)
{
	// Handle Player Collisions/Interactions
	for (int i = 0; i < actors.size(); i++) {
		if (actors[i] != nullptr && doActorsCollide(iceman, actors[i])) {
			//iceman->interactWith(actors[i]);
			actors[i]->interactWith(iceman);
		}
	}

}

bool StudentWorld::doActorsCollide(const Actor* a1, const Actor* a2) const
{
	if (a1 == nullptr || a2 == nullptr)
		return false;

	int x1 = a1->getX();
	int y1 = a1->getY();
	int size1 = a1->getHitBoxSize();

	int x2 = a2->getX();
	int y2 = a2->getY();
	int size2 = a2->getHitBoxSize();

	// Checks if 2D boxes overlap
	bool noOverlap = (x1 + size1 - 1 < x2 || x2 + size2 - 1 < x1 ||
		y1 + size1 - 1 < y2 || y2 + size2 - 1 < y1);

	return !noOverlap;

}

bool StudentWorld::willActorsCollide(int x1, int y1, int size1, const Actor* a2) const
{
	if (a2 == nullptr)
		return false;

	int x2 = a2->getX();
	int y2 = a2->getY();
	int size2 = a2->getHitBoxSize();

	// Checks if 2D boxes overlap
	bool noOverlap = (x1 + size1 - 1 < x2 || x2 + size2 - 1 < x1 ||
		y1 + size1 - 1 < y2 || y2 + size2 - 1 < y1);

	return !noOverlap;
}

void StudentWorld::updateAllActors()
{
	for (int i = 0; i < actors.size(); i++) {
		Actor* a = actors[i];
		if (a == nullptr || !a->isActive())
			continue;

		a->update();

		// make Goodies near player visible:
		if (a->isEnvironmentObject())
			if (calculateDistance(iceman, a) <= 4)
				a->setVisible(true);

		if (a->hasGravity()) {
			bool isStable = checkBelowForIce(a);

			if (isStable && a->getState() == "falling") // Boulder Crashes into Ice
				a->setState("crashed");

			else if (isStable)
				a->setState("stable");
			else if (!isStable && a->getState() == "stable")
				a->setState("waiting");
			// else if
				// Boulder sets self to falling once it's ready
		}
	}
}

bool StudentWorld::checkBelowForIce(Actor* a)
{
	int spriteSize = a->getHitBoxSize();
	int x = a->getX();
	int y = a->getY() - 1;
	for (int i = 0; i < spriteSize; i++) {
		if (ice2DArray[x + i][y] != nullptr)
			return true;
	}

	return false;
}

void StudentWorld::mineIce()
{
	Actor::Direction d = iceman->getDirection();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();
	int iceX = 0;
	int iceY = 0;

	switch (d) {
	case Actor::Direction::right:
		iceX = icemanX + 3;
		iceY = icemanY - 1;

		for (int i = 0; i < 4; i++) {
			iceY++;
			if (iceX >= 0 && iceX < ICE_ARRAY_SIZE && iceY >= 0 && iceY < ICE_ARRAY_SIZE)
				auxMineIce(ice2DArray[iceX][iceY], iceX, iceY);
		}
		break;
	case Actor::Direction::left:
		iceX = icemanX;
		iceY = icemanY - 1;

		for (int i = 0; i < 4; i++) {
			iceY++;
			if (iceX >= 0 && iceX < ICE_ARRAY_SIZE && iceY >= 0 && iceY < ICE_ARRAY_SIZE)
				auxMineIce(ice2DArray[iceX][iceY], iceX, iceY);
		}
		break;
	case Actor::Direction::down:
		iceX = icemanX - 1;
		iceY = icemanY;

		for (int i = 0; i < 4; i++) {
			iceX++;
			if (iceX >= 0 && iceX < ICE_ARRAY_SIZE && iceY >= 0 && iceY < ICE_ARRAY_SIZE)
				auxMineIce(ice2DArray[iceX][iceY], iceX, iceY);
		}
		break;
	case Actor::Direction::up:
		iceX = icemanX - 1;
		iceY = icemanY + 3;

		for (int i = 0; i < 4; i++) {
			iceX++;
			if (iceX >= 0 && iceX < ICE_ARRAY_SIZE && iceY >= 0 && iceY < ICE_ARRAY_SIZE)
				auxMineIce(ice2DArray[iceX][iceY], iceX, iceY);
		}
		break;
	}
}

void StudentWorld::auxMineIce(Ice* iceToBreak, int iceX, int iceY)
{
	if (iceToBreak != nullptr) {
		iceToBreak->setActive(false);
		playSound(SOUND_DIG);
	}
}

int StudentWorld::determineGameStatus()
{
	if (iceman->getHitPoints() <= 0)
		return GWSTATUS_PLAYER_DIED;

	else if (numBarrelsOfOil <= 0) {
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::useSonarKit(Actor* a1)
{
	for (Actor* a2 : actors) {
		if (a1 == nullptr || a2 == nullptr)
			continue;

		if (a2->isGoodieObject() && calculateDistance(a1, a2) < 100)
			a2->setVisible(true);
	}
}


// general helper functions:
double StudentWorld::calculateDistance(const Actor* a1, const Actor* a2) const {
	if (a1 == nullptr || a2 == nullptr) {
		return 0;
	}
	return calculateDistance(a1->getX(), a1->getY(), a1->getSize(), a2->getX(), a2->getY(), a2->getSize());
}

double StudentWorld::calculateDistance(int x, int y, double a1Size, const Actor* a2) const {
	if (a2 == nullptr)
		return 0;

	return calculateDistance(x, y, a1Size, a2->getX(), a2->getY(), a2->getSize());
}

double StudentWorld::calculateDistance(int x1, int y1, double a1Size, int x2, int y2, double a2Size) const {
	double centerX1 = x1 + a1Size / 2.0;
	double centerY1 = y1 + a1Size / 2.0;

	double centerX2 = x2 + a2Size / 2.0;
	double centerY2 = y2 + a2Size / 2.0;

	double xDiff = centerX2 - centerX1;
	double yDiff = centerY2 - centerY1;

	return sqrt(xDiff * xDiff + yDiff * yDiff);
}


