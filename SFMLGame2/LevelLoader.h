/*
Currently this class loads .tmx (Tiled maps) and GameObjectManager loads Entities from other XMLs
As a result the Getters in this class MUST be used to move all Entities created from the .tmx to the GameObjectManager.
The getters WILL delete all the elements in the corresponding vectors.
*/

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "SFML\Graphics.hpp"
#include "tinyxml2.h"
#include "custom_defininitions.h"
#include "EntityFactory.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

class LevelLoader
{
public:

	bool CreateLevel(int); //Loades level by index provided
	bool ClearLevel(int);

	void Draw(sf::RenderWindow& rw);

	vector<StrongEntityPtr> GetForegroundTiles(); //Get vector of foreground tiles
	vector<StrongEntityPtr> GetBackgroundTiles(); //Get vector of background tiles
	vector<StrongEntityPtr> GetCollisionEntities(); //Get vector of collision boxes

private:
	bool ReadParameters();
	bool LoadLevel();
	void CreateTile(int, int, string);
	void CreateTile(int, int, int, int, string);
	void GenerateObject(string, int[]);

	enum LayerType { ObjectLayer, TileLayer };

	tinyxml2::XMLDocument m_levelFile;

	vector<vector<int>> m_levelMatrix;
	int m_levelWidth;
	int m_levelHeight;
	int m_textureWidth;
	int m_textureHeight;
	int m_playerStartX;
	int m_playerStartY;
	int m_tileSize;
	int m_currentLevel;

	EntityFactory m_entityFactory;

	sf::Texture m_currentTexture;
	string m_currentTextureName;

	vector<StrongEntityPtr> m_backgroundTiles;
	vector<StrongEntityPtr> m_foregroundTiles;
	vector<StrongEntityPtr> m_collisionEntities;

	//Map is tile index, width and height of object
	static const map<int, pair<int, int>> m_staticObjectIdentifiers;
};

#endif