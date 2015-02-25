#include "EntityFactory.h"
#include "Entity.h"
#include "ComponentBase.h"

#include "InputComponent.h"
#include "InventoryComponent.h"
#include "ItemComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

EntityFactory::EntityFactory()
{
	m_componentFactory.Register<TransformComponent>(ComponentBase::GetIDFromName(TransformComponent::COMPONENT_NAME));
	m_componentFactory.Register<RenderComponent>(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME));
	m_componentFactory.Register<CollisionComponent>(ComponentBase::GetIDFromName(CollisionComponent::COMPONENT_NAME));
	m_componentFactory.Register<InputComponent>(ComponentBase::GetIDFromName(InputComponent::COMPONENT_NAME));
}

StrongEntityPtr EntityFactory::CreateEntity(XMLElement* currEntityNode)
{
	
	std::string entityType = currEntityNode->Attribute("type");
	StrongEntityPtr newEntity(new Entity(GetNextEntityID(), entityType.c_str()));

	XMLElement* currComponentNode = currEntityNode->FirstChildElement("Component");

	while (currComponentNode)
	{
		StrongComponentPtr newComponent = CreateComponent(currComponentNode);

		if (newComponent)
		{
			newEntity->AddComponent(newComponent);
			newComponent->SetOwner(newEntity);
		}
		else
		{
			return StrongEntityPtr();
		}
		currComponentNode = currComponentNode->NextSiblingElement("Component");
	}

	newEntity->PostInit();

	return newEntity;
}

StrongEntityPtr EntityFactory::CreateTileFromTmx(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect)
{
	/*
	TODO
	Temporary solution for creating tiles
	*/

	//std::string entityType = currEntityNode->Attribute("type");
	StrongEntityPtr newEntity(new Entity(GetNextEntityID(), "Tile"));

	TransformComponent transComp;
	transComp.SetMovable(false);
	transComp.SetPosition(position);
	transComp.SetSpeed(sf::Vector2f(0, 0));
	transComp.SetOwner(newEntity);
	StrongComponentPtr newComp(std::make_shared<TransformComponent>(transComp));
	newEntity->AddComponent(newComp);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);

	RenderComponent renderComp;
	renderComp.SetSprite(sprite);
	renderComp.SetOwner(newEntity);
	newComp = std::make_shared<RenderComponent>(renderComp);
	newEntity->AddComponent(newComp);
	newComp->PostInit();

	return newEntity;
	
}

StrongEntityPtr EntityFactory::CreateCollisionEntity(sf::Vector2f position, sf::Vector2f dimensions)
{
	StrongEntityPtr newEntity(new Entity(GetNextEntityID(), "Collision"));

	TransformComponent transComp;
	transComp.SetMovable(false);
	transComp.SetPosition(position);
	transComp.SetSpeed(sf::Vector2f(0, 0));
	transComp.SetOwner(newEntity);
	StrongComponentPtr newComp(std::make_shared<TransformComponent>(transComp));
	newEntity->AddComponent(newComp);

	sf::FloatRect collisionBounds;
	collisionBounds.left = position.x;
	collisionBounds.top = position.y;
	collisionBounds.width = dimensions.x;
	collisionBounds.height = dimensions.y;

	CollisionComponent collComp;
	collComp.SetBounds(collisionBounds);
	collComp.SetOwner(newEntity);
	newComp = std::make_shared<CollisionComponent>(collComp);
	newEntity->AddComponent(newComp);

	return newEntity;
}

StrongComponentPtr EntityFactory::CreateComponent(XMLElement* currComponentNode)
{
	StrongComponentPtr newComponent(m_componentFactory.Create(ComponentBase::GetIDFromName(currComponentNode->Attribute("name"))));

	if (newComponent)
	{
		if (!newComponent->Init(currComponentNode))
		{
			//ERROR
			return StrongComponentPtr();
		}
		//TODO add init code
	}
	else
	{
		return StrongComponentPtr();
	}

	return newComponent;
}

EntityID EntityFactory::m_lastEntityID = 0;