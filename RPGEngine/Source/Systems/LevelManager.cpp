/*
The methods of creating Entities and Components here are temporary
*/
#include "LevelManager.h"
#include "..\Utilities\Debug.h"
#include "EntityFactory.h"
#include "..\Entity.h"

#include <string>
#include <sstream>

using tinyxml2::XMLElement;

LevelManager::LevelManager(EntityManager& em) : m_entityManager(em)
{
	
}

bool LevelManager::CreateLevel(int levelID)
{
	bool result;

	string filename = "data/levels/level" + std::to_string(levelID) + ".tmx";
	tinyxml2::XMLError error = m_levelFile.LoadFile(filename.c_str());
	m_currentLevel = levelID;

	result = ReadParameters();
	if (!result)
	{
		return false;
	}

	Debug::StartTimer("LLDEBUG_Reading tiles: ");

	result = LoadLevel();
	if (!result)
	{
		return false;
	}

	Debug::EndTimer("end: ");
	return true;
}

bool LevelManager::ReadParameters()
{
	//TODO add errors

	tinyxml2::XMLNode* pRoot = m_levelFile.FirstChildElement("map");
	XMLElement* pMapParam = pRoot->ToElement();

	pMapParam->QueryAttribute("width", &m_levelWidth);
	pMapParam->QueryAttribute("height", &m_levelHeight);
	pMapParam->QueryAttribute("tileheight", &m_tileSize);

	pMapParam = pRoot->FirstChildElement("tileset");
	XMLElement* pTilesetParam = pMapParam->FirstChildElement("image");
	m_currentTextureName = pTilesetParam->Attribute("source");
	pTilesetParam->QueryAttribute("width", &m_textureWidth);
	pTilesetParam->QueryAttribute("height", &m_textureHeight);

	m_currentTexture.loadFromFile("data/levels/" + m_currentTextureName);

	return true;
}

bool LevelManager::LoadLevel()
{
	//Add errors

	m_levelMatrix.resize(m_levelHeight);
	for (int i = 0; i < m_levelHeight; ++i)
	{
		m_levelMatrix[i].resize(m_levelWidth);
	}
		
	int worldIndex = 0;
	int tileIndex;
	string  layerName;

	tinyxml2::XMLNode* pRoot = m_levelFile.FirstChildElement("map");
	XMLElement* pLayer = pRoot->FirstChildElement("layer");

	//Load Tiles
	while (pLayer != nullptr)
	{
		layerName = pLayer->Attribute("name");
		XMLElement* pData = pLayer->FirstChildElement("data");
		XMLElement* pTile = pData->FirstChildElement("tile");
		worldIndex = 0;
		while (pTile != nullptr)
		{
			worldIndex++;
			pTile->QueryAttribute("gid", &tileIndex);
			if (tileIndex != 0)
			{
				int i = floor((worldIndex - 1) / m_levelWidth);
				int j = (worldIndex - 1) % m_levelWidth;
				m_levelMatrix[i][j] = tileIndex;
				map<int, pair<int, int>>::const_iterator mapElement = m_staticObjectIdentifiers.find(tileIndex);

				if (mapElement != m_staticObjectIdentifiers.end())
				{
					CreateTile(worldIndex, tileIndex, mapElement->second.first, mapElement->second.second, layerName);
				}
				else
				{
					CreateTile(worldIndex, tileIndex, layerName);
				}
			}
			pTile = pTile->NextSiblingElement("tile");
		}

		pLayer = pLayer->NextSiblingElement("layer");
	}

	//Load objects
	int values[4];
	XMLElement* pObjectGroup = pRoot->FirstChildElement("objectgroup");
	XMLElement* pObject;

	while (pObjectGroup != nullptr)
	{
		layerName = pObjectGroup->Attribute("name");

		pObject = pObjectGroup->FirstChildElement("object");

		while (pObject != nullptr)
		{
			GenerateObject(pObject, layerName);
			pObject = pObject->NextSiblingElement("object");
		}

		pObjectGroup = pRoot->NextSiblingElement("objectGroup");
	}


	return true;
}

void LevelManager::CreateTile(int worldIndex, int tileIndex, string depth)
{
	int i = floor((worldIndex - 1) / m_levelWidth);
	int j = (worldIndex - 1) % m_levelWidth;
	int layer;
	int textureI = floor((tileIndex - 1) / (m_textureWidth / m_tileSize));
	int textureJ = (tileIndex - 1) % (m_textureWidth / m_tileSize);

	bool solid = true;
	if ((tileIndex == 80) || (tileIndex == 87))
		solid = false;

	sf::IntRect subRect;
	int ty = textureJ*m_tileSize;
	int tx = textureI*m_tileSize;
	subRect.top = tx;
	subRect.left = ty;
	subRect.width = m_tileSize;
	subRect.height = m_tileSize;

	int y = i*m_tileSize, x = j *m_tileSize;
	StrongEntityPtr newEntity = m_entityManager.CreateEntity();
	
	if (depth.compare("Foreground") == 0)
		layer = 1;
	else
		layer = 0;

	EntityFactory::GetInstance()->CreateTileFromTmx(sf::Vector2f(x, y), m_currentTexture, subRect,layer, newEntity);
	newEntity->PostInit();
}

void LevelManager::CreateTile(int worldIndex, int tileIndex, int width, int height, string depth)
{
	int layer;
	int i = floor((worldIndex - 1) / m_levelWidth);
	int j = (worldIndex - 1) % m_levelWidth;

	int textureI = floor((tileIndex - 1) / (m_textureWidth / m_tileSize));
	int textureJ = (tileIndex - 1) % (m_textureWidth / m_tileSize);

	sf::IntRect subRect;
	int ty = textureJ*m_tileSize;
	int tx = textureI*m_tileSize;
	subRect.top = tx;
	subRect.left = ty;
	subRect.width = width*m_tileSize;
	subRect.height = height*m_tileSize;

	int y = i*m_tileSize, x = j *m_tileSize;
	StrongEntityPtr newEntity = m_entityManager.CreateEntity();
	if (depth.compare("Foreground") == 0)
		layer = 1;
	else
		layer = 0;
	EntityFactory::GetInstance()->CreateTileFromTmx(sf::Vector2f(x, y), m_currentTexture, subRect, layer, newEntity);
	newEntity->PostInit();
}

void LevelManager::GenerateObject(XMLElement* pObject, string layerName)
{
	StrongEntityPtr newObject;
	int values[4];
	values[0] = pObject->IntAttribute("x");
	values[1] = pObject->IntAttribute("y");
	values[2] = pObject->IntAttribute("width");
	values[3] = pObject->IntAttribute("height");

	if (layerName == "Collision")
	{
		auto pPolyline = pObject->FirstChildElement("polyline");
		if (!pPolyline)
		{
			PhysicsDefs::RectangleData data;
			data.center.x = values[0];
			data.center.y = values[1];
			data.width = values[2];
			data.height = values[3];
			EntityFactory::GetInstance()->CreateCollisionEntity(sf::Vector2f(values[0], values[1]), &data, 1, m_entityManager.CreateEntity());
		}
		else
		{
			glm::vec2 point;
			//Stream for parsing the polyline
			std::stringstream polyStream(pPolyline->Attribute("points"));
			//Contains comma delimited values of the points
			std::vector<string> tokenVector;
			string token;

			PhysicsDefs::PolygonData data;

			while (polyStream >> token)
			{
				tokenVector.clear();
				std::size_t prev = 0, pos;
				while ((pos = token.find_first_of(",", prev)) != std::string::npos)
				{
					if (pos > prev)
						tokenVector.push_back(token.substr(prev, pos - prev));
					prev = pos + 1;
				}
				if (prev < token.length())
					tokenVector.push_back(token.substr(prev, std::string::npos));

				point.x = values[0] + std::stof(tokenVector[0]);
				point.y = values[1] + std::stof(tokenVector[1]);
				data.points.push_back(point);
			}
			EntityFactory::GetInstance()->CreateCollisionEntity(sf::Vector2f(values[0], values[1]), &data, 1, m_entityManager.CreateEntity());
		}			
	}

	return;
}

//vector<int> LevelManager::PossibleTiles(sf::Vector2f objectPosition)
//{
//	int objectI, objectJ;
//	vector<int> result;
//	int x = objectPosition.x, y = objectPosition.y;
//
//	objectJ = floor(x / m_tileSize);
//	objectI = floor(y / m_tileSize);
//
//	int startI = objectI - 5, endI = objectI + 5, startJ = objectJ - 5, endJ = objectJ + 5;
//
//	if (startI < 0)
//		startI = 0;
//	else if (endI > m_levelHeight)
//		endI = m_levelHeight;
//	if (startJ < 0)
//		startJ = 0;
//	else if (endJ > m_levelWidth)
//		endJ = m_levelWidth;
//
//	for (int i = startI; i < endI; ++i)
//	{
//		for (int j = startJ; j < endJ; ++j)
//		{
//			int c = m_levelMatrix[i][j];
//			if (c > 0)
//				result.push_back(c);
//		}
//	}
//
//	return result;
//}

const map<int, pair<int, int>> LevelManager::m_staticObjectIdentifiers =
{
	{ 811, { 2, 4 } },
	{ 809, { 2, 4 } },
	{ 807, { 2, 4 } },
	{ 805, { 2, 4 } },
	{ 803, { 2, 4 } },
	{ 801, { 2, 4 } },
	{ 771, { 2, 1 } },
	{ 729, { 2, 2 } },
	{ 767, { 2, 1 } },
	{ 725, { 2, 2 } },
	{ 763, { 2, 1 } },
	{ 721, { 2, 2 } },
};