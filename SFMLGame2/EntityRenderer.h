#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "custom_defininitions.h"
#include "RenderComponent.h"

using std::vector;

class EntityRenderer
{
public:
	EntityRenderer(sf::RenderWindow& rw) : m_renderWindow(rw)
	{
		vector<std::shared_ptr<RenderComponent>> vec;
		m_renderComponents.push_back(vec);
		m_renderComponents.push_back(vec);
	};

	void DivideIntoLayers();

	void Update(float dt);
	
	void Draw();
	static StrongComponentPtr CreateRenderComponent();
private:
	void ZSortEntities();
	static bool CompareBottom(std::shared_ptr<RenderComponent>& entity1, std::shared_ptr<RenderComponent>& entity2);
	sf::RenderWindow& m_renderWindow;
	static vector<vector<std::shared_ptr<RenderComponent>>> m_renderComponents;

	int m_framesSinceLastSort = 0;
	const int MAX_SORT_FRAME_INTERVAL = 5;
};

#endif