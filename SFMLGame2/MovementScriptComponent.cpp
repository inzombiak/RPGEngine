#include "TransformComponent.h"
#include "MovementScriptComponent.h"
#include "LuaStateManager.h"
#include "Entity.h"
#include "custom_defininitions.h"

bool MovementScriptComponent::Init(XMLElement* node)
{
	if (!node->Attribute("filepath"))
		return false;
	m_scriptFilePath = node->Attribute("filepath");
	auto pLuaState = LuaStateManager::GetInstance();
	pLuaState->ExecuteFile(m_scriptFilePath.c_str());

	RegisterScriptFunctions();

	m_scriptObject = pLuaState->GetGlobalVars().CreateTable("table1");
	LuaPlus::LuaObject metaTableObj = pLuaState->GetGlobalVars().Lookup(METATABLE_NAME);
	if (metaTableObj.IsNil())
	{
		//Error handling needed
		return false;
	}
	m_scriptObject.SetLightUserData("__object", this);
	m_scriptObject.SetObject("__index", metaTableObj);
	m_scriptObject.SetMetaTable(metaTableObj);

	m_updateFunction = m_scriptObject.GetByName("Update");
	if (!m_updateFunction.IsFunction())
		return false;
	return true;
}

void MovementScriptComponent::Update(float dt)
{
	if (m_updateFunction.IsFunction())
	{
		LuaPlus::LuaFunction<void> update(m_updateFunction);
		update(m_scriptObject, dt);
	}
	
}

void MovementScriptComponent::RegisterScriptFunctions()
{
	auto pLuaState = LuaStateManager::GetInstance();
	LuaPlus::LuaObject metaTableObj = pLuaState->GetGlobalVars().Lookup(METATABLE_NAME);
	if (metaTableObj.IsNil())
	{
		//Error handling needed
		return;
	}
	metaTableObj.SetObject("__index", metaTableObj);
	metaTableObj.RegisterObjectDirect("GetPos", (MovementScriptComponent*)0, &MovementScriptComponent::GetPos);
	metaTableObj.RegisterObjectDirect("SetPos", (MovementScriptComponent*)0, &MovementScriptComponent::SetPos);
}

LuaPlus::LuaObject MovementScriptComponent::GetPos(void)
{
	LuaPlus::LuaObject ret;

	std::shared_ptr<TransformComponent> pTransformComponent;
	if (CheckConvertAndCastPtr<ComponentBase, TransformComponent>(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)), pTransformComponent))
		LuaStateManager::GetInstance()->ConvertSFMLVecToTable(pTransformComponent->GetPosition(), ret);
	else
		ret.AssignNil(LuaStateManager::GetInstance()->GetLuaState());
	return ret;
}

void MovementScriptComponent::SetPos(LuaPlus::LuaObject newPos)
{
	std::shared_ptr<TransformComponent> pTransformComponent;
	if (CheckConvertAndCastPtr<ComponentBase, TransformComponent>(m_owner->GetComponent(GetIDFromName(TransformComponent::COMPONENT_NAME)), pTransformComponent))
	{
		sf::Vector2f pos;
		LuaStateManager::GetInstance()->ConvertTableToSFMLVec(newPos, pos);
		pTransformComponent->SetPosition(pos);
	}
	else
	{
		//GCC_ERROR("Attempting to call SetPos() on an actor with no physcial component; ActorId: " + ToStr(m_pOwner->GetId()));
	}
}


const char* MovementScriptComponent::COMPONENT_NAME = "MovementScriptComponent";
const char* MovementScriptComponent::METATABLE_NAME = "MovementScript";