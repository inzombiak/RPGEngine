#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <Windows.h>

#include "SFML\Graphics.hpp"

#include "custom_defininitions.h"
#include "Entity.h"
#include "tinyxml2.h"

#include <string>
#include <vector>

using std::vector;
using std::string;
class StatComponent;
class VitalsComponent;
class EntityManager
{
public:
	EntityManager();
	void Update(float);
	//void Draw(sf::RenderWindow&);
	StrongEntityPtr CreateEntity();
	void LoadEntities(string filename);
	static StrongComponentPtr CreateVitalsComponent();
	static StrongComponentPtr CreateStatComponent();
	bool CreateEntityAtPosition(const string& entityName, sf::Vector2f pos);
private:
	void Move(float);
	void TestForCollision();
	vector<StrongEntityPtr> m_entities; //Container for "full" Entities e.g. enemies, items etc.
	static vector<std::shared_ptr<VitalsComponent>> m_entityVitals; //Container for  VitalsComponents
	static vector<std::shared_ptr<StatComponent>> m_entityStats; //Container for StatComponent
	tinyxml2::XMLDocument m_entityDefinitionFile; //File containing entity definitions
	std::map<EntityNameID, tinyxml2::XMLElement*> m_entityDefinitions; //map that pairs an entity name with its definition
};

#endif