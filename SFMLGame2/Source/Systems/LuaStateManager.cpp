#include "LuaStateManager.h"

#ifdef _DEBUG
#pragma comment( lib, "..\\Debug\\LUAPlus.lib")
#else
#pragma comment( lib, "..\\Release\\LUAPlus.lib")
#endif

#include "..\Utilities\GameDefs.h"

bool LuaStateManager::Create()
{
	if (s_luaStateManager)
	{
		delete s_luaStateManager;
	}

	s_luaStateManager = new LuaStateManager;
	if (s_luaStateManager)
		return s_luaStateManager->Init();
	return false;
}

void LuaStateManager::Destroy()
{
	delete s_luaStateManager;
}

LuaStateManager::LuaStateManager()
{
	m_luaState = NULL;
}

LuaStateManager::~LuaStateManager()
{
	if (m_luaState)
	{
		LuaPlus::LuaState::Destroy(m_luaState);
		m_luaState = NULL;
	}
}

bool LuaStateManager::Init()
{
	m_luaState = LuaPlus::LuaState::Create(true);
	if (!m_luaState)
		return false;

	m_luaState->GetGlobals().RegisterDirect("ExecuteFile", (*this), &LuaStateManager::ExecuteFile);
	m_luaState->GetGlobals().RegisterDirect("ExecuteString", (*this), &LuaStateManager::ExecuteString);

	return true;
}

void LuaStateManager::ExecuteFile(const char* path)
{
	int result = m_luaState->DoFile(path);
	if (result != 0)
		SetError(result);
}

void LuaStateManager::ExecuteString(const char* chunk)
{
	int result = 0;
	if (strlen(chunk) <= 1 || chunk[0] != '=')
	{
		result = m_luaState->DoString(chunk);
		if (result != 0)
			SetError(result);
	}
	else
	{
		std::string buffer("print(");
		buffer += (chunk + 1);
		buffer += ")";
		result = m_luaState->DoString(buffer.c_str());
		if (result != 0)
			SetError(result);
	}
}

void LuaStateManager::SetError(int errorNum)
{

	LuaPlus::LuaStackObject stackObj(m_luaState, -1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		m_lastError = errStr;
		ClearStack();
	}
	else
		m_lastError = "Unknown Lua parse error";
}

void LuaStateManager::ClearStack()
{
	m_luaState->SetTop(0);
}

LuaPlus::LuaObject LuaStateManager::GetGlobalVars()
{
	return m_luaState->GetGlobals();
}

LuaPlus::LuaState* LuaStateManager::GetLuaState() const
{
	return m_luaState;
}

LuaPlus::LuaObject LuaStateManager::CreatePath(const char* pathString, bool toIgnoreLastElement)
{
	std::vector<std::string> splitPath;
	SplitString(pathString, splitPath, '.');
	if (toIgnoreLastElement)
		splitPath.pop_back();

	LuaPlus::LuaObject context = GetGlobalVars();
	for (auto it = splitPath.begin(); it != splitPath.end(); ++it)
	{
		// make sure we still have a valid context
		if (context.IsNil())
		{
			//GCC_ERROR("Something broke in CreatePath(); bailing out (element == " + (*it) + ")");
			return context;  // this will be nil
		}

		// grab whatever exists for this element
		const std::string& element = (*it);
		LuaPlus::LuaObject curr = context.GetByName(element.c_str());

		if (!curr.IsTable())
		{
			// if the element is not a table and not nil, we clobber it
			if (!curr.IsNil())
			{
				//GCC_WARNING("Overwriting element '" + element + "' in table");
				context.SetNil(element.c_str());
			}

			// element is either nil or was clobbered so add the new table
			context.CreateTable(element.c_str());
		}

		context = context.GetByName(element.c_str());
	}

	// if we get here, we have created the path
	return context;
}

void LuaStateManager::ConvertSFMLVecToTable(const sf::Vector2f& vec, LuaPlus::LuaObject& outLuaTable) const
{
	outLuaTable.AssignNewTable(GetLuaState());
	outLuaTable.SetNumber("x", vec.x);
	outLuaTable.SetNumber("y", vec.y);
}


void LuaStateManager::ConvertTableToSFMLVec(const LuaPlus::LuaObject& luaTable, sf::Vector2f& outVec) const
{
	LuaPlus::LuaObject temp;

	//x
	temp = luaTable.Get("x");
	if (temp.IsNumber())
		outVec.x = temp.GetFloat();
	//else
		//GCC_ERROR("luaTable.x is not a number");

	//y
	temp = luaTable.Get("y");
	if (temp.IsNumber())
		outVec.y = temp.GetFloat();
	//else
		//GCC_ERROR("luaTable.y is not a number");
}

LuaStateManager* LuaStateManager::s_luaStateManager = NULL;