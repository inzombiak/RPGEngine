#include "GameObjectManager.h"


void GameObjectManager::Add(std::string filename, VisibleGameObject* object)
{
	object->Load(filename);
	//m_gameObjects.push_back(object);
}

void GameObjectManager::Destroy()
{
	if (m_Player)
	{
		delete m_Player;
	}

	while (!m_staticGameObjects.empty())
	{
		delete m_staticGameObjects.back();

		m_staticGameObjects.pop_back();
	}

	while (!m_movableGameObjects.empty())
	{
		//delete m_movableGameObjects.back();

		m_movableGameObjects.pop_back();	
	}

	while (!m_levelTiles.empty())
	{
		delete m_levelTiles.back();

		m_levelTiles.pop_back();
	}
}

void GameObjectManager::Update(float time)
{
	m_Player->Update(time);

	for (vector<VisibleGameObject*>::iterator it = m_movableGameObjects.begin(); it != m_movableGameObjects.end(); ++it)
	{
		(*it)->Update(time);
	}

	Move(time);
	//m_Player->Move(0, 1, time);
	//m_Player->Move(1, 0, time);
	//m_Player->End();
	
}

void GameObjectManager::Move(float time)
{
	m_Player->Move(0, 1, time);

	for (vector<VisibleGameObject*>::iterator it = m_staticGameObjects.begin(); it != m_staticGameObjects.end(); ++it)
	{
		if ((*it)->GetSolid())
		{
			if (CheckCollision(m_Player->GetGlobalCollisionRectangle(), (*it)->GetGlobalCollisionRectangle()))
			{
				//m_Player->Stop();
				int r = ResolveCollision((*m_Player), *(*it));
				if (r == 0)
					m_Player->SetGrounded(true);
				else if (r == 1)
					m_Player->SetVelocityY(0);
			}
		}
	}

	m_Player->Move(1, 0, time);
	
	for (vector<VisibleGameObject*>::iterator it = m_staticGameObjects.begin(); it != m_staticGameObjects.end(); ++it)
	{
		if ((*it)->GetSolid())
		{
			if (CheckCollision(m_Player->GetGlobalCollisionRectangle(), (*it)->GetGlobalCollisionRectangle()))
			{
				ResolveCollision((*m_Player), *(*it));
				m_Player->SetVelocityX(0);
			}
		}
	}
}

int GameObjectManager::ResolveCollision(VisibleGameObject& obj1, VisibleGameObject& obj2)
{	
	float obj1HalfHeight = obj1.GetCollisionRectangle().height / 2;
	float obj2HalfHeight = obj2.GetCollisionRectangle().height / 2;

	float obj1HalfWidth = obj1.GetCollisionRectangle().width / 2;
	float obj2HalfWidth = obj2.GetCollisionRectangle().width / 2;

	float obj1X = obj1.GetPosition().x + obj1.GetCollisionRectangle().left + obj1HalfWidth;
	float obj1Y = obj1.GetPosition().y + obj1.GetCollisionRectangle().top + obj1HalfHeight;
	float obj2X = obj2.GetPosition().x + obj2.GetCollisionRectangle().left + obj2HalfWidth;
	float obj2Y = obj2.GetPosition().y + obj2.GetCollisionRectangle().top + obj2HalfHeight;

	float xDistance = abs(obj1X - obj2X);
	float yDistance = abs(obj1Y - obj2Y);

	float xOverlap = obj1HalfWidth + obj2HalfWidth - xDistance;
	float yOverlap = obj1HalfHeight + obj2HalfHeight - yDistance;

	if (abs(yOverlap) < 0.001)
		yOverlap = 0.001;

	int result = -1;

	if (abs(xOverlap) > abs(yOverlap))
	{
		xOverlap = 0;
		if (obj1Y < obj2Y)
		{
			yOverlap *= -1;
			result = 0;
		}
		else
			result = 1;
	}
	else
	{
		result = 3;
		yOverlap = 0;
		if (obj1X < obj2X)
		{
			xOverlap *= -1;
		}
		else
		{
		}
	}
	obj1.SetPosition(obj1.GetPosition().x + xOverlap, obj1.GetPosition().y + yOverlap);
	return result;
}

void GameObjectManager::Draw(sf::RenderWindow &rw)
{
	for (vector<VisibleGameObject*>::iterator it = m_staticGameObjects.begin(); it != m_staticGameObjects.end(); ++it)
	{
		(*it)->Draw(rw);
	}
	m_renderer.Draw(rw);
}

bool GameObjectManager::CheckCollision(sf::Rect<float>& r1, sf::Rect<float>& r2)
{
	bool result = r1.intersects(r2);
	return result;
}

void GameObjectManager::SetPlayer(int x, int y, string filename)
{
	m_Player = new Player();
	m_Player->SetPosition(x, y);
	m_Player->Load(filename);
}

void GameObjectManager::SetStaticObjects(vector<VisibleGameObject*>& objects)
{
	m_staticGameObjects = objects;
}

void GameObjectManager::SetMovableObjects(vector<VisibleGameObject*>& objects)
{
	m_movableGameObjects = objects;
}

void GameObjectManager::SetTiles(vector<VisibleGameObject*>& tiles)
{
	m_levelTiles = tiles;
	m_renderer.SetStaticObjects(tiles);
	m_renderer.SetPlayer(m_Player);
}

Player* GameObjectManager::GetPlayer()
{
	return m_Player;
}

bool GameObjectManager::GetLevelStatus()
{
	return m_isLevelComplete;
}

void GameObjectManager::SetLevelStatus(bool b)
{
	m_isLevelComplete = b;
}

vector<VisibleGameObject*> GameObjectManager::GetPossibleCollisions(VisibleGameObject& object)
{
	vector<int> possibleCollTileIndicies = LevelLoader::PossibleTiles(object.GetPosition());
	vector<VisibleGameObject*> result;

	while (!possibleCollTileIndicies.empty())
	{
		result.push_back(m_levelTiles[possibleCollTileIndicies.back()]);
		possibleCollTileIndicies.pop_back();
	}

	return result;
}