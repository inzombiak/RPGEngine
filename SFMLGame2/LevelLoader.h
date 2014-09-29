#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "Debug.h"

#include "Tile.h"
#include "ObjectFactory.h"

using namespace std;

class LevelLoader
{
public:
	bool CreateLevel(int);

	bool ClearLevel(int);

	void Destroy();
	void Draw(sf::RenderWindow&);
	void GetPlayerStart(int&, int&);
	sf::Vector2f GetLevelSize();

	//static int**& GetLevelMatrix();

	static vector<VisibleGameObject*>& GetLevelTiles();
	static vector<VisibleGameObject*>& GetLevelStaticObjects();
	static vector<VisibleGameObject*>& GetLevelMovableObjects();
	static vector<int> PossibleTiles(sf::Vector2f);
	//static void GetLevelParameters(int, int, int, int);

private:
	static bool ReadParameters(string);
	static bool LoadLevel(string);
	static void CreateTile(int, int, string);
	static void GenerateObject(string, int[]);
	static void CreateTile(int, int, int, int, string);
	enum LayerType { ObjectLayer, TileLayer };

	static int** m_levelMatrix;
	static int m_levelWidth;
	static int m_levelHeight;
	static int m_textureWidth;
	static int m_textureHeight;
	static int m_playerStartX;
	static int m_playerStartY;
	static int m_tileSize;
	static int m_currentLevel;
	static sf::Texture m_currentTexture;
	static string m_currentTextureName;
	static vector<VisibleGameObject*> m_backgroundTiles;
	static vector<VisibleGameObject*> m_foregroundTiles;
	static vector<VisibleGameObject*> m_collisionObjects;
	static vector<VisibleGameObject*> m_interactableObjects;
	static vector<VisibleGameObject*> m_staticObjects;

	//Map is tile index, width and height of object
	static const map<int, pair<int, int>> m_staticObjectIdentifiers;
};

#endif