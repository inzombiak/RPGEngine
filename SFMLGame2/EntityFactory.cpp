#include "EntityFactory.h"
#include "Entity.h"

#include "ComponentFactory.h"

#include "ComponentBase.h"
#include "InputComponent.h"
#include "InventoryComponent.h"
#include "PickupComponent.h"
#include "ItemManager.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

#include "TransformManager.h"
#include "InputManager.h"
#include "EntityRenderer.h"
#include "ItemManager.h"
#include "PhysicsManager.h"

EntityFactory::EntityFactory()
{
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(TransformComponent::COMPONENT_NAME), TransformManager::CreateTransformComponent);
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME), EntityRenderer::CreateRenderComponent);
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(InputComponent::COMPONENT_NAME), InputManager::CreateInputComponent);
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(InventoryComponent::COMPONENT_NAME), ItemManager::CreateInventoryComponent);
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(ItemPickupComponent::COMPONENT_NAME), ItemManager::CreateItemPickupComponent);
	ComponentFactory::RegisterEntityComponent(ComponentBase::GetIDFromName(CollisionComponent::COMPONENT_NAME), PhysicsManager::CreateCollisionComponent);
	
}

bool EntityFactory::CreateEntity(XMLElement* currEntityNode, StrongEntityPtr newEntity)
{
	std::string entityType = currEntityNode->Attribute("type");;
	newEntity->Init(GetNextEntityID(), entityType.c_str());
	XMLElement* currComponentNode = currEntityNode->FirstChildElement("Component");

	while (currComponentNode)
	{
		StrongComponentPtr newComponent;
		/*
		Find a way of merging item and entity creation
		*/
		newComponent = CreateComponent(currComponentNode);

		if (newComponent)
		{
			newEntity->AddComponent(newComponent);
			newComponent->SetOwner(newEntity);
		}
		else
		{
			//return false;
		}
		currComponentNode = currComponentNode->NextSiblingElement("Component");
	}

	newEntity->PostInit();

	return true;
}

//Entity EntityFactory::CreateEntityByCopy(XMLElement* currEntityNode)
//{
//	std::string entityType = currEntityNode->Attribute("type");
//	Entity newEntity;
//	newEntity.Init(GetNextEntityID(), entityType.c_str());
//	return newEntity;
//}

bool EntityFactory::CreateTileFromTmx(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect,int depth, StrongEntityPtr newEntity)
{
	/*
	TODO
	Temporary solution for creating tiles
	*/

	//std::string entityType = currEntityNode->Attribute("type");
	newEntity->Init(GetNextEntityID(), "Tile");
	StrongComponentPtr newComp = ComponentFactory::CreateEntityComponent(ComponentBase::GetIDFromName(TransformComponent::COMPONENT_NAME));
	std::shared_ptr<TransformComponent> transComp = std::static_pointer_cast<TransformComponent>(newComp);
	transComp->SetMovable(false);
	transComp->SetPosition(position);
	transComp->SetSpeed(sf::Vector2f(0, 0));
	transComp->SetOwner(newEntity);
	newEntity->AddComponent(newComp);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);

	newComp = ComponentFactory::CreateEntityComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME));
	std::shared_ptr<RenderComponent> renderComp = std::static_pointer_cast<RenderComponent>(newComp);
	//renderComp->SetSprite(sprite);
	renderComp->SetOwner(newEntity);
	renderComp->SetTexture(texture, textureRect);
	renderComp->SetDepth(depth);
	newEntity->AddComponent(newComp);
	newComp->PostInit();

	return true;
	
}

bool EntityFactory::CreateCollisionEntity(sf::Vector2f position, sf::Vector2f dimensions,int depth, StrongEntityPtr newEntity)
{
	/*Todo
	Fix this shit
	*/
	newEntity->Init(GetNextEntityID(), "Collision");

	StrongComponentPtr newComp = ComponentFactory::CreateEntityComponent(ComponentBase::GetIDFromName(TransformComponent::COMPONENT_NAME));
	std::shared_ptr<TransformComponent> transComp = std::static_pointer_cast<TransformComponent>(newComp);
	transComp->SetMovable(false);
	transComp->SetPosition(position);
	transComp->SetSpeed(sf::Vector2f(0, 0));
	transComp->SetOwner(newEntity);
	newEntity->AddComponent(newComp);

	sf::FloatRect collisionBounds;
	collisionBounds.left = position.x;
	collisionBounds.top = position.y;
	collisionBounds.width = dimensions.x;
	collisionBounds.height = dimensions.y;

	/*CollisionComponent collComp;
	collComp.SetBounds(collisionBounds);
	collComp.SetOwner(newEntity);
	newComp = std::make_shared<CollisionComponent>(collComp);
	newEntity->AddComponent(newComp);*/

	return true;
}

StrongComponentPtr EntityFactory::CreateComponent(XMLElement* currComponentNode)
{
	StrongComponentPtr newComponent = ComponentFactory::CreateEntityComponent(ComponentBase::GetIDFromName(currComponentNode->Attribute("name")));
	if (!newComponent || !newComponent->Init(currComponentNode))
		return StrongComponentPtr();
	return newComponent;
}

EntityID EntityFactory::m_lastEntityID = 0;