#include "LevelLoader.h"

bool LevelLoader::CreateLevel(int levelID)
{
	bool result;
	string filename = "data/levels/level" + to_string(levelID) + ".tmx";
	m_currentLevel = levelID;
	result = ReadParameters(filename);
	if (!result)
	{
		return false;
	}
	Debug::StartTimer("LLDEBUG_Reading tiles: ");
	result = LoadLevel(filename);
	if (!result)
	{
		return false;
	}
	Debug::EndTimer("end: ");
	return true;
}

bool LevelLoader::ReadParameters(string filename)
{
	ifstream f;
	string garbage;
	char input;

	f.open(filename);
	if (f.fail() == true)
	{
		return false;
	}

	getline(f, garbage);
	garbage = "";
	//Read Level Width
	f.get(input);
	while (input != 'w')
	{
		f.get(input);
	}
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_levelWidth = atoi(garbage.c_str());
	garbage = "";

	//Read Level Height
	f.get(input);
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_levelHeight = atoi(garbage.c_str());
	garbage = "";

	//Read Tile Size
	f.get(input);
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_tileSize = atoi(garbage.c_str());
	garbage = "";

	getline(f, garbage);
	getline(f, garbage);

	//Get Texture Name
	garbage = "";
	f.get(input);
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_currentTextureName = garbage;
	garbage = "";

	//Get Texture Width
	f.get(input);
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_textureWidth = atoi(garbage.c_str());
	garbage = "";

	//Get Texture Height
	f.get(input);
	while (input != '"')
	{
		f.get(input);
	}
	f.get(input);
	while (input != '"')
	{
		garbage += input;
		f.get(input);
	}
	m_textureHeight = atoi(garbage.c_str());
	garbage = "";
	m_currentTexture.loadFromFile("data/levels/" + m_currentTextureName);
	f.close();
	
	return true;
}

bool LevelLoader::LoadLevel(string filename)
{
	ifstream f;
	int worldIndex = 0, tileIndex = 0;
	int values[4];
	char input = '1';
	string garbage, layerName, name;
	LayerType layerType;
	bool endOfLayer = false;

	m_levelMatrix = new int*[m_levelHeight];
	for (int i = 0; i < m_levelHeight; ++i)
		m_levelMatrix[i] = new int[m_levelWidth]();

	f.open(filename);

	getline(f, garbage);
	getline(f, garbage);
	getline(f, garbage);
	getline(f, garbage);
	getline(f, garbage);
	
	//Read layers
	while (true)
	{
		garbage = "";
		while (input != '<' )
		{
			f.get(input);
		}
		
		f.get(input);
		while (input != ' ')
		{
			garbage += input;
			f.get(input);
			if (f.eof())
			{
				break;
			}
		}
		if (f.eof())
		{
			break;
		}

		if (garbage == "objectgroup")
		{
			layerType = LayerType::ObjectLayer;
		}
		else if (garbage == "layer")
		{
			layerType = LayerType::TileLayer;
		}

		while (input != '"')
		{
			f.get(input);
		}

		f.get(input);
		garbage = "";
		while (input != '"')
		{
			garbage += input;
			f.get(input);
		}

		layerName = garbage;
		endOfLayer = false;
		getline(f, garbage);
		garbage = "";
		if (layerType == LayerType::TileLayer)
		{
			worldIndex = 0;
			getline(f, garbage);
			while (true)
			{
				garbage = "";
				f.get(input);
				while (input != '<')
				{
					f.get(input);
				}
				
				f.get(input);
				if (input == )
				while (input != '"')
				{
					garbage += input;
					f.get(input);
					if (input == '\n')
					{
						f.get(input);
						garbage = "";
					}						
					if (garbage == " </layer")
					{
						endOfLayer = true;
						break;
					}
				}
				if (endOfLayer)
				{
					break;
				}
				worldIndex++;
				f.get(input);
				if (input != '0')
				{
					garbage = "";
					while (input != '"')
					{
						garbage += input;
						f.get(input);
					}

					tileIndex = atoi(garbage.c_str());
					int i = floor((worldIndex - 1) / m_levelWidth);
					int j = (worldIndex - 1) % m_levelWidth;
					m_levelMatrix[i][j] = tileIndex;
					CreateTile(worldIndex, tileIndex, layerName);
				}
				while (input != '\n')
				{
					f.get(input);
				}
			}
		}
		else if (layerType == LayerType::ObjectLayer)
		{
			while (true)
			{
				while (input != '<')
				{
					f.get(input);
				}

				garbage = "";
				f.get(input);

				while (input != ' ')
				{
					garbage += input;
					f.get(input);
					if (garbage == "/object")
					{
						endOfLayer = true;
						break;
					}
				}
				if (endOfLayer)
				{
					break;
				}
				if (garbage == "object")
				{
					//Get object name
					while (input != '"')
					{
						f.get(input);
					}
					f.get(input);
					name = "";
					while (input != '"')
					{
						name += input;
						f.get(input);
					}

					//Get object x, y, witdth and height
					for (int i = 0; i < 4; ++i)
					{
						f.get(input);
						garbage = "";
						while (input != '"')
						{
							f.get(input);
						}
						f.get(input);
						while (input != '"')
						{
							garbage += input;
							f.get(input);
						}
						values[i] = atoi(garbage.c_str());
					}
					GenerateObject(layerName, values);
				}
				while (input != '\n')
				{
					f.get(input);
				}

			}
			cout << garbage;
		}
	}

	f.close();
	return true;
}

void LevelLoader::CreateTile(int worldIndex, int tileIndex, string depth)
{
	int i = floor((worldIndex - 1) / m_levelWidth);
	int j = (worldIndex - 1) % m_levelWidth;

	int textureI = floor((tileIndex - 1) / (m_textureWidth / 32));
	int textureJ = (tileIndex - 1) % (m_textureWidth / 32);

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

void LevelLoader::GenerateObject(string objectName, int values[])
{
	if (objectName == "Player")
	{
		m_playerStartX = values[0];
		m_playerStartY = values[1];
		return;
	}
	VisibleGameObject* newObject = ObjectFactory::GenerateObject(objectName, values);
	if (newObject)
	{
		m_collisionObjects.push_back(newObject);
	}
		

	return;
}

void LevelLoader::Draw(sf::RenderWindow& rw)
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
	}

	for (vector<VisibleGameObject*>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); ++it)
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

void LevelLoader::GetPlayerStart(int& x, int& y)
{
	x = m_playerStartX;
	y = m_playerStartY;
}

void LevelLoader::Destroy()
{
	for (int i = 0; i < m_levelHeight; ++i)
	{
		delete [] m_levelMatrix[i];
	}
	delete [] m_levelMatrix;

	//m_interactableObjects.erase(m_interactableObjects.begin(), m_interactableObjects.end());
	//m_collisionObjects.erase(m_collisionObjects.begin(), m_collisionObjects.end());
	m_backgroundTiles.erase(m_backgroundTiles.begin(), m_backgroundTiles.end());
}
/*
int**& LevelLoader::GetLevelMatrix()
{
	return m_levelMatrix;
}*/

vector<VisibleGameObject*>& LevelLoader::GetLevelTiles()
{
	return m_foregroundTiles;
}

vector<VisibleGameObject*>& LevelLoader::GetLevelStaticObjects()
{
	return m_collisionObjects;
}

vector<VisibleGameObject*>& LevelLoader::GetLevelMovableObjects()
{
	return m_collisionObjects;
}

sf::Vector2f LevelLoader::GetLevelSize()
{
	return sf::Vector2f(m_levelWidth * m_tileSize, m_levelHeight * m_tileSize);
}

vector<int> LevelLoader::PossibleTiles(sf::Vector2f objectPosition)
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

int** LevelLoader::m_levelMatrix;
int LevelLoader::m_tileSize;
vector<VisibleGameObject*> LevelLoader::m_foregroundTiles;
vector<VisibleGameObject*> LevelLoader::m_backgroundTiles;
vector<VisibleGameObject*> LevelLoader::m_collisionObjects;
vector<VisibleGameObject*> LevelLoader::m_interactableObjects;
int LevelLoader::m_levelHeight;
int LevelLoader::m_levelWidth;