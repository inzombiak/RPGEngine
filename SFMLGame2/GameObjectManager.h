#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "EntityRenderer.h"
#include "EntityFactory.h"
#include <Windows.h>

#include <string>
#include <vector>

using std::vector;
using std::string;

class GameObjectManager
{
public:

	void Update(float);
	void Draw(sf::RenderWindow&);
	void LoadEntities(string filename);
	void SetForegroundTiles(std::vector<StrongEntityPtr>&);
	void SetBackgroundTiles(vector<StrongEntityPtr>&);
	void SetCollisionBoxes(std::vector<StrongEntityPtr>&);
	WeakEntityPtr GetPlayer();

private:
	void Move(float);
	void TestForCollision();
	void ZSortEntities(); //Z-sorts objects for proper redering. Must be called before passing entities to renderer. Only sorts m_entities
	static bool CompareBottom(StrongEntityPtr& entity1, StrongEntityPtr& entity2); //Comparison function for Z-sorting

	//EntityRenderer m_renderer;
	EntityFactory m_entityFactory;
	/*
	Entities are sperated to make certain operations simpler
	*/
	StrongEntityPtr m_player; //Player entity
	vector<StrongEntityPtr> m_entities; //Container for "full" Entities e.g. enemies, items etc.
	vector<StrongEntityPtr> m_backgroundTiles; //Container for backgorund tiles
	vector<StrongEntityPtr> m_colllisionBoxes; //Container for collison boxes

	//Used for Z-sort. Sorting is done once every 5 frames.
	int m_framesSinceLastSort = 0;
	const int MAX_SORT_FRAME_INTERVAL = 5;
};

#endif