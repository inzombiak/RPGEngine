#include "VisibleGameObject.h"
#include "Player.h"
#include "LevelLoader.h"
#include "ObjectRenderer.h"
#include <Windows.h>

#include <string>
#include <vector>

using std::vector;

class GameObjectManager
{
public:
	void Add(std::string, VisibleGameObject*);
	void Update(float);
	
	void Draw(sf::RenderWindow&);
	void Destroy();

	//void GetObject(std::string)

	void SetStaticObjects(std::vector<VisibleGameObject*>&);
	void SetMovableObjects(vector<VisibleGameObject*>&);
	void SetTiles(std::vector<VisibleGameObject*>&);
	void SetPlayer(int, int, string);
	Player* GetPlayer();

	bool GetLevelStatus();
	void SetLevelStatus(bool);

	bool CheckCollision(sf::Rect<float>&, sf::Rect<float>&);
	//int ResolveCollision(VisibleGameObject*, VisibleGameObject*, int, int);
	int ResolveCollision(VisibleGameObject&, VisibleGameObject&);

private:

	void Move(float);
	void TestForCollision();
	vector<VisibleGameObject*> GetPossibleCollisions(VisibleGameObject&);

	vector<VisibleGameObject*> m_staticGameObjects;
	vector<VisibleGameObject*> m_movableGameObjects;
	vector<VisibleGameObject*> m_levelTiles;
	Player* m_Player;
	ObjectRenderer m_renderer;
	bool m_isLevelComplete;
};