#pragma once

#include "..\..\LuaPlus\LuaPlus.h"
#include "SFML\Graphics.hpp"

#include <string>

/*
Based on implementation in Game Coding Complete 4
*/

class LuaStateManager
{
	static LuaStateManager* s_luaStateManager;
	LuaPlus::LuaState* m_luaState;
	std::string m_lastError;

public:
	// Singleton functions
	static bool Create();
	static void Destroy();
	static LuaStateManager* GetInstance() 
	{	
		assert(s_luaStateManager); 
		return s_luaStateManager; 
	}

	// IScriptManager interface
	bool Init();
	void ExecuteFile(const char* resource);
	void ExecuteString(const char* str);

	LuaPlus::LuaObject GetGlobalVars();
	LuaPlus::LuaState* GetLuaState() const;

	// public helpers
	LuaPlus::LuaObject CreatePath(const char* pathString, bool toIgnoreLastElement = false);
	void ConvertSFMLVecToTable(const sf::Vector2f& vec, LuaPlus::LuaObject& outLuaTable) const;
	void ConvertTableToSFMLVec(const LuaPlus::LuaObject& luaTable, sf::Vector2f& outVec3) const;

private:
	void SetError(int errorNum);
	void ClearStack(void);

	// private constructor & destructor; call the static Create() and Destroy() functions instead
	explicit LuaStateManager(void);
	virtual ~LuaStateManager(void);
};

