#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "VisibleGameObject.h"
#include "Algorithms.h"
#include <vector>

class ObjectRenderer
{
public:
	void Draw(sf::RenderWindow&);

	void SetPlayer(VisibleGameObject*);
	void SetStaticObjects(std::vector<VisibleGameObject*>&);
	void SetMovableObjects(std::vector<VisibleGameObject*>&);

private:
	void Sort(std::vector<VisibleGameObject*>&);
	bool CompareVisibleGameObjectsBottom(const VisibleGameObject*&, const VisibleGameObject*&);
	static bool CompareBottom(VisibleGameObject*&, VisibleGameObject*&);
	void Swap(VisibleGameObject*&, VisibleGameObject*&);
	std::vector<VisibleGameObject*> m_staticGameObjects;
	std::vector<VisibleGameObject*> m_movableGameObjects;
	int m_frameCount = 0;
	const int m_maxFrameCount = 5;
	VisibleGameObject* m_player;
};