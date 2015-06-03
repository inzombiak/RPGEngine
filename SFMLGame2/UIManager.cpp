#include "UIManager.h"
#include "ItemOptionsMenu.h"
#include "ComponentFactory.h"

#include "Entity.h"

#include "PlayerObserverComponent.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.h"
#include "ItemComponent.h"
#include "VitalsComponent.h"
#include "StatComponent.h"
#include "CharacterWindow.h"

UIManager::UIManager()
{
	//Register Item Render creation function with component factory
	ComponentFactory::RegisterItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME), CreateItemRenderComponent);
	//Initalization for vitals bars, will steamline
	m_playerHPBarParameters.left = CONST_HPBarX;
	m_playerHPBarParameters.top = CONST_HPBarY;
	m_playerHPBarParameters.height = CONST_HPBarHeight;

	m_playerHPBar.setPosition(CONST_HPBarX, CONST_HPBarY);

	m_playerHPBarFillParameters.left = CONST_HPBarX;
	m_playerHPBarFillParameters.top = CONST_HPBarY;
	m_playerHPBarFillParameters.height = CONST_HPBarHeight;

	m_playerHPBarFill.setPosition(CONST_HPBarX, CONST_HPBarY);

	m_playerHPBar.setFillColor(CONST_HPBarColor);
	m_playerHPBarFill.setFillColor(CONST_HPBarFillColor);

	m_playerStaminaBarParameters.left = CONST_StaminaBarX;
	m_playerStaminaBarParameters.top = CONST_StaminaBarY;
	m_playerStaminaBarParameters.height = CONST_StaminaBarHeight;

	m_playerStaminaBar.setPosition(CONST_StaminaBarX, CONST_StaminaBarY);

	m_playerStaminaBarFillParameters.left = CONST_StaminaBarX;
	m_playerStaminaBarFillParameters.top = CONST_StaminaBarY;
	m_playerStaminaBarFillParameters.height = CONST_StaminaBarHeight;

	m_playerStaminaBarFill.setPosition(CONST_StaminaBarX, CONST_StaminaBarY);

	m_playerStaminaBar.setFillColor(CONST_StaminaBarColor);
	m_playerStaminaBarFill.setFillColor(CONST_StaminaBarFillColor);
	m_font.loadFromFile("data/arial.ttf");

	newOpMenu = std::shared_ptr<ItemOptionsMenu>(new ItemOptionsMenu(m_font));
	m_characterWindow = std::shared_ptr<CharacterWindow>(new CharacterWindow(m_font, 18));
	m_characterWindow->Init();
}

StrongItemComponentPtr UIManager::CreateItemRenderComponent()
{
	std::shared_ptr<ItemRenderComponent> newRenderComp(new ItemRenderComponent());
	m_itemRenderComponents.push_back(newRenderComp);
	return newRenderComp;
}

void UIManager::PostInit()
{
	//Get the components the UI needs
	StrongEntityPtr player = ConvertToStrongPtr(m_playerObserver->GetOwner());
	if (player)
	{
		//Get players inventory
		CheckConvertAndCastPtr(player->GetComponent(ComponentBase::GetIDFromName(InventoryComponent::COMPONENT_NAME)), m_playerInventory);

		//Get players vitals
		std::shared_ptr<VitalsComponent> playerVitals;
		if (CheckConvertAndCastPtr(player->GetComponent(ComponentBase::GetIDFromName(VitalsComponent::COMPONENT_NAME)), playerVitals))
		{
			VitalsComponent::Vital hp, stamina;
			if (playerVitals->GetVital(VitalsComponent::VitalType::Health, hp))
			{
				SetPlayerHP(hp.current, hp.max);
			}
			if (playerVitals->GetVital(VitalsComponent::VitalType::Stamina, stamina))
			{
				SetPlayerStamina(stamina.current, stamina.max);
			}
		}

		//Get players stats
		std::shared_ptr<StatComponent> playerStats;
		if (CheckConvertAndCastPtr(player->GetComponent(ComponentBase::GetIDFromName(StatComponent::COMPONENT_NAME)), playerStats))
			m_characterWindow->SetStats(playerStats);

		//Get player equipment
		CheckConvertAndCastPtr<ComponentBase, EquipmentComponent>(player->GetComponent(ComponentBase::GetIDFromName(EquipmentComponent::COMPONENT_NAME)), m_playerEquipment);
	}
}

StrongComponentPtr UIManager::CreatePlayerObserverComponent()
{
	m_playerObserver = std::shared_ptr<PlayerObserverComponent>(new PlayerObserverComponent);
	return m_playerObserver;
}

void UIManager::Update(float dt)
{
	UpdatePlayerVitals();
	if (m_playerObserver->EquipmentOpen())
		m_characterWindow->Update(dt);
}

void UIManager::UpdatePlayerVitals()
{
	StrongEntityPtr player = ConvertToStrongPtr(m_playerObserver->GetOwner());
	if (player)
	{
		WeakComponentPtr wVitalsPtr = player->GetComponent(ComponentBase::GetIDFromName(VitalsComponent::COMPONENT_NAME));
		if (!wVitalsPtr.expired())
		{
			std::shared_ptr<VitalsComponent> playerVitals = CastComponentToDerived<StrongComponentPtr, VitalsComponent>(ConvertToStrongPtr(wVitalsPtr));
			VitalsComponent::Vital hp, stamina;
			if (playerVitals->GetVital(VitalsComponent::VitalType::Health, hp))
			{
				SetPlayerHP(hp.current,hp.max);
			}
			if (playerVitals->GetVital(VitalsComponent::VitalType::Stamina, stamina))
			{
				SetPlayerStamina(stamina.current,stamina.max);
			}
		}
	}

}
void UIManager::SetPlayerHP(int curr, int max)
{
	if (m_playerCurrentHP != curr)
	{
		m_playerCurrentHP = curr;
		m_playerHPBarFillParameters.width = m_playerCurrentHP * CONST_HPFillPixelPerPoint;
		m_playerHPBarFill.setSize(sf::Vector2f(m_playerHPBarFillParameters.width, m_playerHPBarFillParameters.height));
	}
	if (m_playerMaxHP != max)
	{
		m_playerMaxHP = max;
		m_playerHPBarParameters.width = m_playerMaxHP * CONST_HPFillPixelPerPoint;
		m_playerHPBar.setSize(sf::Vector2f(m_playerHPBarParameters.width, m_playerHPBarParameters.height));
	}
}

void UIManager::SetPlayerStamina(int curr, int max)
{
	if (m_playerCurrentStamina != curr)
	{
		m_playerCurrentStamina = curr;
		m_playerStaminaBarFillParameters.width = m_playerCurrentStamina * CONST_StaminaFillPixelPerPoint;
		m_playerStaminaBarFill.setSize(sf::Vector2f(m_playerStaminaBarFillParameters.width, m_playerStaminaBarFillParameters.height));
	}
	if (m_playerMaxStamina != max)
	{
		m_playerMaxStamina = max;
		m_playerStaminaBarParameters.width = m_playerMaxStamina * CONST_StaminaFillPixelPerPoint;
		m_playerStaminaBar.setSize(sf::Vector2f(m_playerStaminaBarParameters.width, m_playerStaminaBarParameters.height));
	}	
}
void UIManager::DrawInventory(sf::RenderWindow& rw)
{
	const std::map<ItemID, Item>& items = m_playerInventory->GetItems();
	Item nextItem;
	sf::Text itemInfo;
	itemInfo.setFont(m_font);
	itemInfo.setCharacterSize(18);
	//TODO Create render component, make dynamic
	int i = 40;
	for (std::map<ItemID, Item>::const_iterator it = items.begin(); it != items.end(); ++it)
	{
		nextItem = it->second;
		StrongItemComponentPtr strComp = ConvertToStrongPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME)));
		std::shared_ptr<BaseItemComponent> itemBaseComp= CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(strComp);
		StrongItemComponentPtr renderStrComp = ConvertToStrongPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME)));
		std::shared_ptr<ItemRenderComponent> renderComp = CastComponentToDerived<StrongItemComponentPtr, ItemRenderComponent>(renderStrComp);
		renderComp->SetPosition(sf::Vector2f(20, i));
		itemInfo.setString(itemBaseComp->GetItemName() + "\n" + itemBaseComp->GetDescription() + ", " + std::to_string(itemBaseComp->GetQuantity()));
		itemInfo.setPosition(80, i);
		rw.draw(itemInfo);
		rw.draw(renderComp->GetSprite());
		i += 40;
	}
}

void UIManager::DrawEquipment(sf::RenderWindow& rw)
{
	//Get equipment
	const std::map<Equipment::SlotName, Item> equipment = m_playerEquipment->GetEquipment();

	//Create map of equipment render components, FIX THIS
	std::map<Equipment::SlotName, std::shared_ptr<ItemRenderComponent>> equipmentRenderComponents;
	Item nextItem;
	std::shared_ptr<ItemRenderComponent> renderComp;
	for (auto it = equipment.begin(); it != equipment.end(); ++it)
	{
		nextItem = it->second;
		if (CheckConvertAndCastPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME)), renderComp))
			equipmentRenderComponents[it->first] = renderComp;
	}

	m_characterWindow->Draw(equipmentRenderComponents, rw);
	
}

void UIManager::Draw(sf::RenderWindow& rw)
{
	
	rw.draw(m_playerHPBar);
	rw.draw(m_playerHPBarFill);
	rw.draw(m_playerStaminaBar);
	rw.draw(m_playerStaminaBarFill);
	if (m_playerObserver->InventoryOpen())
		DrawInventory(rw);
	if (m_playerObserver->EquipmentOpen())
		DrawEquipment(rw);
	newOpMenu->Draw(rw);
	//m_playerInventory.Draw(rw);
	
}

void UIManager::HandleInput(sf::Event event, sf::Vector2i mousePos)
{
	Item nextItem;
	//If right click, open options menus
	if (event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2f mousePosFloat(mousePos.x, mousePos.y);

		//Check if mouse is in character window or inventory
		if (m_characterWindow->GetBounds().contains(mousePosFloat))
		{
			const  std::map<Equipment::SlotName, Item>& items = m_playerEquipment->GetEquipment();
			for (auto it = items.begin(); it != items.end(); ++it)
			{
				nextItem = it->second;
				StrongItemComponentPtr renderStrComp = ConvertToStrongPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME)));
				std::shared_ptr<ItemRenderComponent> renderComp = CastComponentToDerived<StrongItemComponentPtr, ItemRenderComponent>(renderStrComp);
				sf::FloatRect spriteBounds = renderComp->GetSprite().getGlobalBounds();
				if (spriteBounds.contains(mousePosFloat))
				{
					StrongItemComponentPtr baseItemComp = ConvertToStrongPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(BaseItemComponent::COMPONENT_NAME)));
					std::shared_ptr<BaseItemComponent> baseItemCompShared = CastComponentToDerived<StrongItemComponentPtr, BaseItemComponent>(baseItemComp);

					ItemID id = reinterpret_cast<ItemID>(HashedString::hash_name(baseItemCompShared->GetItemName().c_str()));

					newOpMenu->SetPosition(mousePosFloat);
					newOpMenu->SetOpen(true);
					newOpMenu->AttachItem(id);
				}
			}
		}
		else
		{
			const std::map<ItemID, Item>& items = m_playerInventory->GetItems();
			Item nextItem;
			for (std::map<ItemID, Item>::const_iterator it = items.begin(); it != items.end(); ++it)
			{
				nextItem = it->second;
				StrongItemComponentPtr renderStrComp = ConvertToStrongPtr(nextItem.GetItemComponent(ItemComponent::GetIDFromName(ItemRenderComponent::COMPONENT_NAME)));
				std::shared_ptr<ItemRenderComponent> renderComp = CastComponentToDerived<StrongItemComponentPtr, ItemRenderComponent>(renderStrComp);
				sf::FloatRect spriteBounds = renderComp->GetSprite().getGlobalBounds();
				if (spriteBounds.contains(mousePosFloat))
				{
					newOpMenu->SetPosition(mousePosFloat);
					newOpMenu->SetOpen(true);
					newOpMenu->AttachItem(it->first);
				}
			}
		}
	}
	else if (event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePosFloat(mousePos.x, mousePos.y);
		if (newOpMenu->IsOpen())
		{
			int response = newOpMenu->HandleInput(event, mousePosFloat); //0 = out of obunds, 1 = use, 2 = drop, 3 = delete;
			if (response == 0)
				newOpMenu->SetOpen(false);
			else
			{
				if (m_characterWindow->GetBounds().contains(mousePosFloat))
				{
					if (response == 1)
						m_playerEquipment->Unequip(newOpMenu->GetAttachedItem(), nextItem);

					m_playerInventory->AddItem(nextItem, 1);
				}
				else
				{
					if (response == 1)
						m_playerInventory->UseItem(newOpMenu->GetAttachedItem());
					else if (response == 2)
						m_playerInventory->DropItem(newOpMenu->GetAttachedItem());
					else if (response == 3)
						m_playerInventory->DeleteItem(newOpMenu->GetAttachedItem());

				}

				newOpMenu->SetOpen(false);
			}
		}
	}
}

std::shared_ptr<PlayerObserverComponent> UIManager::m_playerObserver;
std::vector<std::shared_ptr<ItemRenderComponent>> UIManager::m_itemRenderComponents;
std::map<Equipment::SlotName, sf::Vector2f> UIManager::m_slotDisplayCoordinates =
{
	{ Equipment::LeftHand, sf::Vector2f(400, 40)},
	{ Equipment::RightHand, sf::Vector2f{ 320, 40 } },
	{ Equipment::Armor, sf::Vector2f{ 360, 40 } },
};