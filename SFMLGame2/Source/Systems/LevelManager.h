/*
Currently this class loads .tmx (Tiled maps) and GameObjectManager loads Entities from other XMLs
As a result the Getters in this class MUST be used to move all Entities created from the .tmx to the GameObjectManager.
The getters WILL delete all the elements in the corresponding vectors.
*/

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "SFML\Graphics.hpp"
#include "..\Utilities\tinyxml2.h"
#include "..\Utilities\GameDefs.h"
#include "EntityManager.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

class LevelManager
{
public:
	LevelManager(EntityManager&);
	bool CreateLevel(int); //Loades level by index provided
	bool ClearLevel(int);

private:
	bool ReadParameters();
	bool LoadLevel();
	void CreateTile(int, int, string);
	void CreateTile(int, int, int, int, string);
	void GenerateObject(tinyxml2::XMLElement*, string);

	enum LayerType { ObjectLayer, TileLayer };

	tinyxml2::XMLDocument m_levelFile;

	EntityManager& m_entityManager;

	vector<vector<int>> m_levelMatrix;
	int m_levelWidth;
	int m_levelHeight;
	int m_textureWidth;
	int m_textureHeight;
	int m_playerStartX;
	int m_playerStartY;
	int m_tileSize;
	int m_currentLevel;

	sf::Texture m_currentTexture;
	string m_currentTextureName;
	//Map is tile index, width and height of object
	static const map<int, pair<int, int>> m_staticObjectIdentifiers;
};

#endif