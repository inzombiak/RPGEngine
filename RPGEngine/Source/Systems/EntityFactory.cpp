#include "EntityFactory.h"
#include "..\Entity.h"

#include "..\Components\ComponentFactory.h"

#include "..\Components\IComponent.h"
#include "..\Components\InputComponent.h"
#include "..\Components\InventoryComponent.h"
#include "..\Components\PickupComponent.h"
#include "..\Components\RenderComponent.h"
#include "..\Components\CollisionComponent.h"
#include "..\Components\TransformComponent.h"
#include "..\Components\PlayerObserverComponent.h"
#include "..\Components\VitalsComponent.h"
#include "..\Components\StatComponent.h"
#include "..\Components\EquipmentComponent.h"
#include "..\Components\MovementScriptComponent.h"
#include "..\Components\AnimationComponent.h"

#include "..\Systems\ItemManager.h"
#include "..\UI\UIManager.h"
#include "..\Systems\TransformManager.h"
#include "..\Systems\InputManager.h"
#include "..\Rendering\RenderManager.h"
#include "..\Systems\ItemManager.h"
#include "..\Systems\EntityManager.h"
#include "..\Physics\PhysicsManager.h"
#include "..\Systems\ScriptManager.h"
#include "..\Rendering\AnimationManager.h"

EntityFactory::EntityFactory()
{
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(TransformComponent::COMPONENT_NAME), TransformManager::CreateTransformComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(RenderComponent::COMPONENT_NAME), RenderManager::CreateRenderComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(InputComponent::COMPONENT_NAME), InputManager::CreateInputComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(InventoryComponent::COMPONENT_NAME), ItemManager::CreateInventoryComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(EquipmentComponent::COMPONENT_NAME), ItemManager::CreateEquipmentComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(ItemPickupComponent::COMPONENT_NAME), ItemManager::CreateItemPickupComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(CollisionComponent::COMPONENT_NAME), PhysicsManager::CreateCollisionComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(PlayerObserverComponent::COMPONENT_NAME), UIManager::CreatePlayerObserverComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(VitalsComponent::COMPONENT_NAME), EntityManager::CreateVitalsComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(StatComponent::COMPONENT_NAME), EntityManager::CreateStatComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(MovementScriptComponent::COMPONENT_NAME), ScriptManager::CreateMovementScriptComponent);
	ComponentFactory::RegisterEntityComponent(IComponent::GetIDFromName(AnimationComponent::COMPONENT_NAME), AnimationManager::CreateAnimationComponent);
}

bool EntityFactory::CreateEntity(XMLElement* currEntityNode, StrongEntityPtr newEntity)
{
	std::string entityName = currEntityNode->Attribute("name");
	EntityNameID id = GetEntityNameIDFromName(entityName);
	newEntity->Init(GetNextEntityID(), id);
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
bool EntityFactory::CreateTileFromTmx(sf::Vector2f position, sf::Texture& texture, sf::IntRect textureRect,int depth, StrongEntityPtr newEntity)
{
	/*
	TODO
	Temporary solution for creating tiles
	*/

	//std::string entityType = currEntityNode->Attribute("type");
	newEntity->Init(GetNextEntityID(), GetEntityNameIDFromName("Tile"));
	StrongComponentPtr newComp = ComponentFactory::CreateEntityComponent(IComponent::GetIDFromName(TransformComponent::COMPONENT_NAME));
	std::shared_ptr<TransformComponent> transComp = std::static_pointer_cast<TransformComponent>(newComp);
	transComp->SetMovable(false);
	transComp->SetPosition(position);
	transComp->SetSpeed(sf::Vector2f(0, 0));
	transComp->SetOwner(newEntity);
	newEntity->AddComponent(newComp);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);

	newComp = ComponentFactory::CreateEntityComponent(IComponent::GetIDFromName(RenderComponent::COMPONENT_NAME));
	std::shared_ptr<RenderComponent> renderComp = std::static_pointer_cast<RenderComponent>(newComp);
	renderComp->SetSprite(sprite);
	renderComp->SetOwner(newEntity);
	renderComp->SetTexture(texture, textureRect);
	renderComp->SetDepth(depth);
	newEntity->AddComponent(newComp);
	newComp->PostInit();

	return true;
	
}

bool EntityFactory::CreateCollisionEntity(sf::Vector2f position, PhysicsDefs::IShapeData* data, int depth, StrongEntityPtr newEntity)
{
	/*Todo
	Fix this shit
	*/
	newEntity->Init(GetNextEntityID(), GetEntityNameIDFromName("Collision"));

	StrongComponentPtr newComp = ComponentFactory::CreateEntityComponent(IComponent::GetIDFromName(TransformComponent::COMPONENT_NAME));
	std::shared_ptr<TransformComponent> transComp = std::static_pointer_cast<TransformComponent>(newComp);
	transComp->SetMovable(false);
	transComp->SetPosition(position);
	transComp->SetSpeed(sf::Vector2f(0, 0));
	transComp->SetOwner(newEntity);
	newEntity->AddComponent(newComp);

	PhysicsDefs::CollisionShape collShape = data->GetShape();

	shared_ptr<CollisionComponent> collComp = CastComponentToDerived<StrongComponentPtr, CollisionComponent>(ComponentFactory::CreateEntityComponent(IComponent::GetIDFromName(CollisionComponent::COMPONENT_NAME)));
	collComp->SetBounds(data);
	collComp->SetOwner(newEntity);
	collComp->SetSolid(true);
	collComp->SetCollisionShapeType(collShape);
	newEntity->AddComponent(collComp);

	return true;
}

StrongComponentPtr EntityFactory::CreateComponent(XMLElement* currComponentNode)
{
	StrongComponentPtr newComponent = ComponentFactory::CreateEntityComponent(IComponent::GetIDFromName(currComponentNode->Attribute("name")));
	if (!newComponent || !newComponent->Init(currComponentNode))
		return StrongComponentPtr();
	return newComponent;
}

int EntityFactory::m_lastEntityID = 0;