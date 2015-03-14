#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <unordered_map>
#include "custom_defininitions.h"

using std::unordered_map;

class ComponentFactory
{
private:
	typedef StrongComponentPtr(*EntityComponentCreationFunc)();
	typedef StrongItemComponentPtr(*ItemComponentCreationFunc)();
public:
	static bool RegisterEntityComponent(ComponentID id, StrongComponentPtr(*CreationFunction)())
	{
		auto find = m_entityCompCreationFunctions.find(id);
		if (find == m_entityCompCreationFunctions.end())
		{
			m_entityCompCreationFunctions[id] = CreationFunction;
			return true;
		}
		return false;
	}

	static StrongComponentPtr CreateEntityComponent(ComponentID id)
	{
		auto find = m_entityCompCreationFunctions.find(id);
		if (find != m_entityCompCreationFunctions.end())
		{
			EntityComponentCreationFunc pFunc = find->second;
			StrongComponentPtr result = (*pFunc)();
			return result;
		}

		return NULL;
	}

	static bool RegisterItemComponent(ComponentID id, StrongItemComponentPtr(*CreationFunction)())
	{
		auto find = m_itemCompCreationFunctions.find(id);
		if (find == m_itemCompCreationFunctions.end())
		{
			m_itemCompCreationFunctions[id] = CreationFunction;
			return true;
		}
		return false;
	}

	static StrongItemComponentPtr CreateItemComponent(ItemComponentID id)
	{
		auto find = m_itemCompCreationFunctions.find(id);
		if (find != m_itemCompCreationFunctions.end())
		{
			ItemComponentCreationFunc pFunc = find->second;
			StrongItemComponentPtr result = (*pFunc)();
			return result;
		}

		return NULL;
	}

	static ComponentFactory* GetInstance()
	{
		static ComponentFactory instance;
		return &instance;
	}

private:
	ComponentFactory();
	static unordered_map<ComponentID, EntityComponentCreationFunc> m_entityCompCreationFunctions;
	static unordered_map<ItemComponentID, ItemComponentCreationFunc> m_itemCompCreationFunctions;
};


#endif
