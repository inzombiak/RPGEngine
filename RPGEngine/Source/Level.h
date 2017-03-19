#ifndef LEVEL_H
#define LEVEL_H

#include "SFML\Graphics.hpp"
#include "Utilities\GameDefs.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class Level
{
public:
	Level();

	void SetParameters(int levelW, int levelH, int textureW, int textureH, int tileSize)
	{
		m_levelWidth = levelW;
		m_levelHeight = levelH;
		m_textureWidth = textureW;
		m_textureHeight = textureH;
		m_tileSize = tileSize;
		
	}

	void SetLevelMatrix(vector<vector<int>> levelMatrix)
	{
		m_levelMatrix = levelMatrix;
	}

	~Level();

private:
	vector<vector<int>> m_levelMatrix;
	int m_levelWidth;
	int m_levelHeight;
	int m_textureWidth;
	int m_textureHeight;
	int m_tileSize;

	vector<StrongEntityPtr> m_backgroundTiles;
	vector<StrongEntityPtr> m_foregroundTiles;
	vector<StrongEntityPtr> m_collisionEntities;
};

#endif
