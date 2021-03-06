#include "InputComponent.h"
#include "TransformComponent.h"
#include "PlayerObserverComponent.h"
#include "AnimationComponent.h"
#include "SFML\Window.hpp"
#include "..\Entity.h"

bool InputComponent::Init(XMLElement* componentNode)
{
	return true;
}


void InputComponent::Update(float dt, sf::Event event)
{
	StrongComponentPtr compPtr = ConvertToStrongPtr<IComponent>(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)));
	std::shared_ptr<TransformComponent> transComp = CastComponentToDerived<StrongComponentPtr,TransformComponent>(compPtr);
	float vx = 0, vy = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		vx = 100.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		vx = -100.0f;
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
	{
		vy = -100.0f;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
	{
		vy = 100.0f;
	}
	
	if (vx == 0 && vy == 0)
	{
		SwitchAnimation("Idle");
	}

	else if (vx < 0 && vy < 0)
	{
		SwitchAnimation("WalkNW");
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<IComponent>(m_owner->GetComponent(GetIDFromName(PlayerObserverComponent::COMPONENT_NAME)));
		std::shared_ptr<PlayerObserverComponent> invComp = CastComponentToDerived<StrongComponentPtr, PlayerObserverComponent>(compPtr);
		invComp->ToggleInventory();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
	{
		StrongComponentPtr compPtr = ConvertToStrongPtr<IComponent>(m_owner->GetComponent(GetIDFromName(PlayerObserverComponent::COMPONENT_NAME)));
		std::shared_ptr<PlayerObserverComponent> invComp = CastComponentToDerived<StrongComponentPtr, PlayerObserverComponent>(compPtr);
		invComp->ToggleEquipment();
	}


	transComp->SetSpeed(sf::Vector2f(vx, vy));

	return;
}

void InputComponent::SwitchAnimation(const string& message)
{
	std::shared_ptr<AnimationComponent> animComponent;
	if (!CheckConvertAndCastPtr(m_owner->GetComponent(IComponent::GetIDFromName(AnimationComponent::COMPONENT_NAME)), animComponent))
		return;
	
	animComponent->SetAnimation(message);
}

const char* InputComponent::COMPONENT_NAME = "InputComponent";