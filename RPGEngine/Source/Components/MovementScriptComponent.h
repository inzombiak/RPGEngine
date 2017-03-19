#pragma once

#include "IScriptComponent.h"
#include "..\..\LuaPlus\LuaPlus.h"

class MovementScriptComponent : public IScriptComponent
{
	std::string m_scriptObjectName;
	std::string m_scriptFilePath;

	LuaPlus::LuaObject m_scriptObject;
	LuaPlus::LuaObject m_updateFunction;

public:
	static const char* COMPONENT_NAME;
	const char* GetName()
	{ 
		return COMPONENT_NAME; 
	}

	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override{};
	void Update(float dt) override;
	void RegisterScriptFunctions() override;
	void UnregisterScriptFunctions() override {};

private:
	static const char* METATABLE_NAME;
	//Functions required for movement
	LuaPlus::LuaObject GetPos(void);
	void SetPos(LuaPlus::LuaObject newPos);
};

