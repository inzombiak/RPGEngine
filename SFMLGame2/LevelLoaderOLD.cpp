#include "LevelLoaderOLD.h"

bool LevelLoaderOLD::CreateLevel(int levelID)
{
	bool result;
	string filename = "data/levels/level" + to_string(levelID) + ".tmx";
	XMLError error = m_levelFile.LoadFile(filename.c_str());
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

bool LevelLoaderOLD::ReadParameters()
{
	XMLNode* pRoot = m_levelFile.FirstChildElement("map");
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

bool LevelLoaderOLD::LoadLevel()
{
	m_levelMatrix = new int*[m_levelHeight];
	for (int i = 0; i < m_levelHeight; ++i)
		m_levelMatrix[i] = new int[m_levelWidth]();

	int worldIndex = 0;
	int tileIndex;
	string  layerName;

	XMLNode* pRoot = m_levelFile.FirstChildElement("map");
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
			values[0] = pObject->IntAttribute("x");
			values[1] = pObject->IntAttribute("y");
			values[2] = pObject->IntAttribute("width");
			values[3] = pObject->IntAttribute("height");

			GenerateObject(layerName, values);

			pObject = pObjectGroup->NextSiblingElement("object");
		}

		pObjectGroup = pRoot->NextSiblingElement("objectGroup");
	}


	return true;
}

void LevelLoaderOLD::CreateTile(int worldIndex, int tileIndex, string depth)
{
	int i = floor((worldIndex - 1) / m_levelWidth);
	int j = (worldIndex - 1) % m_levelWidth;

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
	Tile* newTile = new Tile();

	newTile->SetPosition(x, y);
	newTile->Load(m_currentTexture, subRect);
	newTile->SetSolid(false);
	newTile->SetVisible(true);

	if (depth == "Background")
	{
		m_backgroundTiles.push_back(newTile);
	}
	else if (depth == "Foreground")
	{
		m_foregroundTiles.push_back(newTile);
	}
	
}

void LevelLoaderOLD::CreateTile(int worldIndex, int tileIndex, int width, int height, string depth)
{

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
	Tile* newTile = new Tile();

	newTile->SetPosition(x, y);
	newTile->Load(m_currentTexture, subRect);
	newTile->SetSolid(false);
	newTile->SetVisible(true);

	if (depth == "Background")
	{
		m_backgroundTiles.push_back(newTile);
	}
	else if (depth == "Foreground")
	{
		m_foregroundTiles.push_back(newTile);
	}
}

void LevelLoaderOLD::GenerateObject(string layerName, int values[])
{
	VisibleGameObject* newObject = NULL;

	if (layerName == "Collision")
	{
		newObject = ObjectFactory::GenerateCollisionBox(values);
	}
	if (layerName == "Player")
	{
		m_playerStartX = values[0];
		m_playerStartY = values[1];
		return;
	}
	
	if (newObject != NULL)
	{
		m_collisionObjects.push_back(newObject);
	}

	return;
}

void LevelLoaderOLD::Draw(sf::RenderWindow& rw)
{
	for (vector<VisibleGameObject*>::iterator it = m_backgroundTiles.begin(); it != m_backgroundTiles.end(); ++it)
	{
		{
			(*it)->Draw(rw);
		}
	}

	/*for (vector<VisibleGameObject*>::iterator it = m_foregroundTiles.begin(); it != m_foregroundTiles.end(); ++it)
	{
		{
			(*it)->Draw(rw);
		}
	}*/

	/*for (vector<VisibleGameObject*>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); ++it)
	{
		(*it)->Draw(rw);
	}

	for (vector<VisibleGameObject*>::iterator it = m_interactableObjects.begin(); it != m_interactableObjects.end(); ++it)
	{
		{
			(*it)->Draw(rw);
		}
	}*/
}

void LevelLoaderOLD::GetPlayerStart(int& x, int& y)
{
	x = m_playerStartX;
	y = m_playerStartY;
}

void LevelLoaderOLD::Destroy()
{
	for (int i = 0; i < m_levelHeight; ++i)
	{
		delete [] m_levelMatrix[i];
	}
	delete [] m_levelMatrix;

	m_interactableObjects.erase(m_interactableObjects.begin(), m_interactableObjects.end());
	m_collisionObjects.erase(m_collisionObjects.begin(), m_collisionObjects.end());
	m_backgroundTiles.erase(m_backgroundTiles.begin(), m_backgroundTiles.end());
}

vector<VisibleGameObject*>& LevelLoaderOLD::GetLevelTiles()
{
	return m_foregroundTiles;
}

vector<VisibleGameObject*>& LevelLoaderOLD::GetLevelStaticObjects()
{
	return m_collisionObjects;
}

vector<VisibleGameObject*>& LevelLoaderOLD::GetLevelMovableObjects()
{
	return m_collisionObjects;
}

sf::Vector2f LevelLoaderOLD::GetLevelSize()
{
	return sf::Vector2f(m_levelWidth * m_tileSize, m_levelHeight * m_tileSize);
}

vector<int> LevelLoaderOLD::PossibleTiles(sf::Vector2f objectPosition)
{
	int objectI, objectJ;
	vector<int> result;
	int x = objectPosition.x, y = objectPosition.y;

	objectJ = floor(x / m_tileSize);
	objectI = floor(y / m_tileSize);

	int startI = objectI - 5, endI = objectI + 5, startJ = objectJ - 5, endJ = objectJ + 5;

	if (startI < 0)
		startI = 0;
	else if (endI > m_levelHeight)
		endI = m_levelHeight;
	if (startJ < 0)
		startJ = 0;
	else if (endJ > m_levelWidth)
		endJ = m_levelWidth;

	for (int i = startI; i < endI; ++i)
	{
		for (int j = startJ; j < endJ; ++j)
		{
			int c = m_levelMatrix[i][j];
			if (c > 0)
				result.push_back(c);
		}
	}

	return result;
}

int** LevelLoaderOLD::m_levelMatrix;
XMLDocument LevelLoaderOLD::m_levelFile;
int LevelLoaderOLD::m_levelWidth;
int LevelLoaderOLD::m_levelHeight;
int LevelLoaderOLD::m_textureWidth;
int LevelLoaderOLD::m_textureHeight;
int LevelLoaderOLD::m_playerStartX;
int LevelLoaderOLD::m_playerStartY;
int LevelLoaderOLD::m_tileSize;
int LevelLoaderOLD::m_currentLevel;
sf::Texture LevelLoaderOLD::m_currentTexture;
string LevelLoaderOLD::m_currentTextureName;
vector<VisibleGameObject*> LevelLoaderOLD::m_backgroundTiles;
vector<VisibleGameObject*> LevelLoaderOLD::m_foregroundTiles;
vector<VisibleGameObject*> LevelLoaderOLD::m_collisionObjects;
vector<VisibleGameObject*> LevelLoaderOLD::m_interactableObjects;
vector<VisibleGameObject*> LevelLoaderOLD::m_staticObjects;
const map<int, pair<int, int>> LevelLoaderOLD::m_staticObjectIdentifiers = 
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