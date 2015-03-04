#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <Windows.h>
#include "custom_defininitions.h"
#include "Entity.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class EntityManager
{
public:
	EntityManager();
	void Update(float);
	//void Draw(sf::RenderWindow&);
	StrongEntityPtr CreateEntity();
	void LoadEntities(string filename);

private:
	void Move(float);
	void TestForCollision();
	//void ZSortEntities(); //Z-sorts objects for proper redering. Must be called before passing entities to renderer. Only sorts m_entities
	//static bool CompareBottom(StrongEntityPtr& entity1, StrongEntityPtr& entity2); //Comparison function for Z-sorting
	/*
	Entities are sperated to make certain operations simpler
	*/
	vector<StrongEntityPtr> m_entities; //Container for "full" Entities e.g. enemies, items etc.
	//Used for Z-sort. Sorting is done once every 5 frames.
};

#endif