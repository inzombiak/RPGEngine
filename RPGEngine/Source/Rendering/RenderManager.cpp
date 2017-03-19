#include "RenderManager.h"

#include "..\Entity.h"
#include "..\Components\IComponent.h"
#include "..\Utilities\Algorithms.h"
#include "..\Utilities\Debug.h"

void RenderManager::DivideIntoLayers()
{
	vector <std::shared_ptr<RenderComponent>> m_layer0;
	int layer;
	
	for (unsigned int i = 0; i < m_renderComponents[0].size(); ++i)
	{
		layer = m_renderComponents[0][i]->GetDepth();
		if (layer == 0)
			m_layer0.push_back(m_renderComponents[0][i]);
		else
		{
			if (m_renderComponents.size()  < layer + 1)
				m_renderComponents.resize(layer + 1);
			m_renderComponents[layer].push_back(m_renderComponents[0][i]);
		}
			
	}
	m_renderComponents[0] = m_layer0;
}

void RenderManager::Update(float dt)
{
	ZSortEntities();
	for (unsigned int i = 0; i < m_renderComponents.size(); ++i)
	{
		for (vector<std::shared_ptr<RenderComponent>>::iterator it = m_renderComponents[i].begin(); it != m_renderComponents[i].end();)
		{
			if (!(*it)->GetInUse())
				it = m_renderComponents[i].erase(it);
			else
			{
				(*it)->Update(dt);
				++it;
			}
		}
	}
}

void RenderManager::Draw()
{
	for (unsigned int i = 0; i < m_renderComponents.size(); ++i)
	{
		for (unsigned int j = 0; j < m_renderComponents[i].size(); j++)
		{	
			if (!m_renderComponents[i][j]->GetInUse())
				m_renderComponents[i].erase(m_renderComponents[i].begin() + j);
			else if (m_renderComponents[i][j]->GetVisible())
				m_renderWindow.draw(m_renderComponents[i][j]->GetSprite());
		}
	}
}

StrongComponentPtr RenderManager::CreateRenderComponent()
{
	std::shared_ptr<RenderComponent> newRenderComp(new RenderComponent());
	m_renderComponents[0].push_back(newRenderComp);
	return newRenderComp;
}

StrongComponentPtr RenderManager::CreateRenderComponentOnLayer(int layer)
{
	std::shared_ptr<RenderComponent> newRenderComp(new RenderComponent());
	m_renderComponents[layer].push_back(newRenderComp);
	return newRenderComp;
}

void RenderManager::ZSortEntities()
{
	m_framesSinceLastSort++;
	//Only sort layer 1, 2 is UI and 0 is background
	if (m_framesSinceLastSort == MAX_SORT_FRAME_INTERVAL)
	{
		m_renderComponents[1] = Algorithms<std::shared_ptr<RenderComponent>>::MergeSortVector(m_renderComponents[1], CompareBottom);
		m_framesSinceLastSort = 0;
	}
}

bool RenderManager::CompareBottom(std::shared_ptr<RenderComponent>& rc1, std::shared_ptr<RenderComponent>& rc2)
{
	int depth1 = rc1->GetDepth();
	int depth2 = rc2->GetDepth();
	float y1 = std::floor(rc1->GetSprite().getPosition().y + rc1->GetSprite().getGlobalBounds().height),
		  y2 = std::floor(rc2->GetSprite().getPosition().y + rc2->GetSprite().getGlobalBounds().height);
	if (depth1 == depth2)
	{
		return y1 <= y2;
	}
	else
		return depth1 < depth2;
}

vector<vector<std::shared_ptr<RenderComponent>>> RenderManager::m_renderComponents;