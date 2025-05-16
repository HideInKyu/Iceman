// StudentWorld.h
// StudentWorld.cpp 
// Actor.h
// Actor.cpp

#include "StudentWorld.h"
#include <string>
using namespace std;
// ---****---
#include <cassert>
#include <vector>
// Forward Declarations:


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

// TODO: Implement init()
int StudentWorld::init() {

	// Initialize ice
	for (int col = 0; col < ICE_ARRAY_SIZE; col++) {
		for (int row = 0; row < ICE_ARRAY_SIZE; row++) {
			ice2DArray[col][row] = new Ice(col, row);
			ice2DArray[col][row]->setVisible(true);
		}
	}

	// Create Tunnel by deactivating Ice 
	for (int col=30; col<=33; col++) {
		for (int row = 4; row <= 59; row++) {
			ice2DArray[col][row]->toggleActive();
		}
	}

	// Set Environment amounts:
	numBoulders = min(currentLevel / 2 + 2, 9);
	numGoldNuggets = max(5 - currentLevel / 2, 2);
	numBarrelsOfOil = min(2 + currentLevel, 21);
	int nonIceOrPlayableActors = numBoulders + numGoldNuggets + numBarrelsOfOil;

	// Distribute Environment Actors
	distributeOilFieldContents(actors);
	for (int i = 0; i < actors.size(); i++) {
		actors[i]->setVisible(true);
	}

	// Set Up Player
	iceman = new Iceman(30, 60, this);
	iceman->setVisible(true);


	// TODO: Figure out when to update level
	// currentLevel++;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// TODO:
	// updateDisplayText();
	// Call update() for all actors
	removeDeadGameObjects();

	// TODO: return proper result:
	// GWSTATUS_PLAYER_DIED
	// GWSTATUS_PLAYER_WON
	// GWSTATUS_FINISHED_LEVEL
	// GWSTATUS_LEVEL_ERROR
	// GWSTATUS_CONTINUE_GAME
	iceman->update();
	mineIce();


	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	for (int col = 0; col < ICE_ARRAY_SIZE; col++) {
		for (int row = 0; row < ICE_ARRAY_SIZE; row++) {
			delete ice2DArray[col][row];
		}
	}
}

// Private functions:
double StudentWorld::calculateDistance(const Actor* a1, const Actor* a2) const{
	double xDiff = a1->getX() - a2->getX();
	double yDiff = a1->getY() - a2->getY();

	xDiff = xDiff * xDiff;
	yDiff = yDiff * yDiff;

	double distance = sqrt(xDiff + yDiff);

	return distance;
}
double StudentWorld::calculateDistance(int x1, int y1, int x2, int y2) const{
	double xDiff = x2 - x1;
	double yDiff = y2 - y1;

	xDiff = xDiff * xDiff;
	yDiff = yDiff * yDiff;

	double distance = sqrt(xDiff + yDiff);

	return distance;
}

void StudentWorld::distributeOilFieldContents(vector<Actor*>& actors)
{
	distributeBoulders(actors);
	distributeGoldNuggets(actors);
	distributeBarrelsOfOil(actors);
}

void StudentWorld::distributeBoulders(vector<Actor*>& actors)
{
	int boulderSpawnX = 0;
	int boulderSpawnY = 0;

	// TODO: MAKE SURE THIS DOES NOT LEAK MEMORY (SHOULDN'T SINCE ITS STATIC MEMORY)
	// TODO: MAKE SURE BOULDER DOES NOT SPAWN IN TUNNELS
	for (int i = 0; i < numBoulders; i++) {
		do {
			boulderSpawnX = static_cast<int> (rand() % (61-4) + 0);
			boulderSpawnY = static_cast<int> (rand() % 37 + 20);
		} while (isTooCloseToOtherActors(boulderSpawnX, boulderSpawnY, actors));

		// Clear 4x4 Ice Around Boulders
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				delete ice2DArray[boulderSpawnX + j][boulderSpawnY + k];
				ice2DArray[boulderSpawnX + j][boulderSpawnY + k] = nullptr;
			}
		}

		actors.push_back(new Boulder(boulderSpawnX, boulderSpawnY));
	}

}

void StudentWorld::distributeGoldNuggets(std::vector<Actor*>& actors)
{
	int goldNuggetSpawnX = 0;
	int goldNuggetSpawnY = 0;

	// TODO: MAKE SURE THIS DOES NOT LEAK MEMORY (SHOULDN'T SINCE ITS STATIC MEMORY)
	// TODO: MAKE SURE GOLDNUGGET DOES NOT SPAWN IN TUNNELS
	for (int i = 0; i < numGoldNuggets; i++) {
		do {
			goldNuggetSpawnX = static_cast<int> (rand() % (61-4) + 0);
			goldNuggetSpawnY = static_cast<int> (rand() % 56 + 0);
		} while (isTooCloseToOtherActors(goldNuggetSpawnX, goldNuggetSpawnY, actors));
		
		actors.push_back(new GoldNugget(goldNuggetSpawnX, goldNuggetSpawnY));
	}
}

void StudentWorld::distributeBarrelsOfOil(std::vector<Actor*>& actors)
{
	int barrelOfOilSpawnX = 0;
	int barrelOfOilSpawnY = 0;

	// TODO: MAKE SURE THIS DOES NOT LEAK MEMORY (SHOULDN'T SINCE ITS STATIC MEMORY)
	// TODO: MAKE SURE BARRELOFOIL DOES NOT SPAWN IN TUNNELS
	for (int i = 0; i < numBarrelsOfOil; i++) {
		do {
			barrelOfOilSpawnX = static_cast<int> (rand() % (61 - 4) + 0);
			barrelOfOilSpawnY = static_cast<int> (rand() % 56 + 0);
		} while (isTooCloseToOtherActors(barrelOfOilSpawnX, barrelOfOilSpawnY, actors));

		actors.push_back(new BarrelOfOil(barrelOfOilSpawnX, barrelOfOilSpawnY));
	}
}

bool StudentWorld::isTooCloseToOtherActors(int x, int y, const std::vector<Actor*>& actors) const
{
	for (int i = 0; i < actors.size(); i++) {
		double distance = calculateDistance(x, y, actors[i]->getX(), actors[i]->getY());
		if (distance < 6)
			return true;
	}

	return false;
}

void StudentWorld::removeDeadGameObjects()
{
	// Delete Ice objects from ice2DArray
	for (int i = 0; i < ICE_ARRAY_SIZE; i++) {
		for (int j = 0; j < ICE_ARRAY_SIZE; j++) {
			if (ice2DArray[i][j] != nullptr && !ice2DArray[i][j]->isActive()) {
				delete ice2DArray[i][j];
				ice2DArray[i][j] = nullptr;
			}
	

		}
	}
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
		iceToBreak->toggleActive();
		playSound(SOUND_DIG);
	}
}

// ***HAS NOT BEEN TESTED YET-- MAY NOT WORK***
bool StudentWorld::doActorsCollide(const Actor* a1, const Actor* a2) const
{
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

//---**** ADD PROTESTOR IMPLEMENTATION HERE ****---