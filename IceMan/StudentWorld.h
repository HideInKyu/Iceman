#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
// ----****----
#include <iostream>
#include <vector>
#include <cmath>
#include "Actor.h"

// Forward Declarations:
class Actor;
class Ice;
class Iceman;


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {}
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// getters
	int getNumBarrlesOfOil() const { return numBarrelsOfOil; }

	// setters/mutators
	void decNumBoulders() { numBoulders--; }
	void decNumGoldNuggets() { numGoldNuggets--; }
	void decNumBarrelsOfOil() { numBarrelsOfOil--; }
	void addActor(Actor* const a) { actors.push_back(a); }

	// Functionality:
	void useSonarKit(Actor* a);
	bool isEmptySpace(Actor* a);
	bool checkCoordsAreValid(int x, int y, int size) const;

private:
	std::vector<Actor*> actors;
	Iceman* iceman;
	const static int ICE_ARRAY_SIZE = 60;
	Ice* ice2DArray[ICE_ARRAY_SIZE][ICE_ARRAY_SIZE];

	int numBoulders;
	int numGoldNuggets;
	int numBarrelsOfOil;

	// init() helpers
	void distributeOilFieldContents(std::vector<Actor*>& actors);
	void distributeBoulders(std::vector<Actor*>& actors);
	void distributeGoldNuggets(std::vector<Actor*>& actors);
	void distributeBarrelsOfOil(std::vector<Actor*>& actors);
	bool isSpawnTooCloseToOtherActors(int x, int y, double size, const std::vector<Actor*>& actors);
	bool isEmptySpace(int x, int y, double objectSize);
	bool isInTunnelSpawn(int x, int y, double size);

	// move() helpers
	void updateDisplayText();
	void removeDeadGameObjects();
	void spawnRandomGoodies();
	void handleCollisions(std::vector<Actor*>& actors);
	bool doActorsCollide(const Actor* a1, const Actor* a2) const;
	bool willActorsCollide(int x1, int y1, int size1, const Actor* a2) const;

	// move helpers-->player interactions
	void mineIce();
	void auxMineIce(Ice* iceToBreak, int x, int y);


	// move() helpers-->updateAllActors() helpers:
	void updateAllActors();
	bool checkBelowForIce(Actor* a);
	int determineGameStatus();

	// general helper functions:
	double calculateDistance(const Actor* a1, const Actor* a2) const;	// Calculate Actors Distance
	double calculateDistance(int x, int y, double a1Size, const Actor* a2) const; // Calculate Actors Distance (a1 is to be spawn, a2 is defined in StudentWorld object)
	double calculateDistance(int x1, int y1, double a1Size, int x2, int y2, double a2Size) const; 	// Calculate "Hypothetical/Numerical" Distances



};

#endif // STUDENTWORLD_H_
