#include "ComponentFactory.h"

unordered_map<ComponentID, ComponentFactory::EntityComponentCreationFunc> ComponentFactory::m_entityCompCreationFunctions;
unordered_map<ItemComponentID, ComponentFactory::ItemComponentCreationFunc> ComponentFactory::m_itemCompCreationFunctions;