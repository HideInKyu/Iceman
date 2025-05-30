#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
// ----****----
#include <vector>
#include <iostream>
#include <cmath>
#include "Actor.h"
// Forward Declarations:
class Actor;
class Iceman;
class Ice;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir): GameWorld(assetDir){}
	virtual ~StudentWorld();

	virtual int init();
		// A. Initialize the data structures used to keep track of your game’s virtual world
		// B.Construct a new oil field that meets the requirements stated in the section below
		// (filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
		// C.Allocate and insert a valid Iceman object into the game world at the proper location

	virtual int move();
	/*
	{
		// move() is ran per tick of game
		// Calls game's actors (Iceman, RegularProtestor, Hardcore Protestor, Boulders, etc. to DOSOMETHING.
		// DELETES actors that !isActive()
		
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	*/

	virtual void cleanUp();
		// TODO: CHECK GAMEWORLD METHODS
		// Called by framework when Player loses a life
		// OR player successfully completes level
		// Frees (Deletes) all Actors


private:
	std::vector<Actor*> actors;
	// TODO: Implement/Define Iceman Class
	 Iceman* iceman;
	// TODO: Declare 2D Array of Ice* -- Array size needed first
	const static int ICE_ARRAY_SIZE = 60;
	Ice* ice2DArray[ICE_ARRAY_SIZE][ICE_ARRAY_SIZE];

	// TODO: move to constructor
	int currentLevel = 0; 
	int numBoulders;
	int numGoldNuggets;
	int numBarrelsOfOil;

	// init() helpers
	// Not on DistributeFunctions -- spawnX = rand() % (61 - 4) --> (-4) prevents spawn OUTSIDE ice array
	void distributeOilFieldContents(std::vector<Actor*>& actors);
	void distributeBoulders(std::vector<Actor*>& actors);
	void distributeGoldNuggets(std::vector<Actor*>& actors);
	void distributeBarrelsOfOil(std::vector<Actor*>& actors);
	bool isTooCloseToOtherActors(int x, int y, const std::vector<Actor*>& actors) const;
	bool isTunnelSpawn(int x, int y) const;

	// move() helper
	void updateDisplayText();
	void handleCollisions(std::vector<Actor*>& actors);
	void removeDeadGameObjects();
	void handleFallingObjects();
	bool checkBelowForIce(Actor* a);
	void spawnRandomGoodies();

	// Actor Interactions:
	bool doActorsCollide(const Actor* a1, const Actor* a2) const;
	void mineIce();
	void auxMineIce(Ice* iceToBreak, int x, int y);

	// Protestor Implementation

	double calculateDistance(const Actor* a1, const Actor* a2) const;	// Calculate Actors Distance
	double calculateDistance(int x1, int x2, int y1, int y2) const; 	// Calculate "Hypothetical/Numerical" Distances
	
};

#endif // STUDENTWORLD_H_
