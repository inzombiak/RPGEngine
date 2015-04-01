#pragma once
#include "ComponentBase.h"
class StatComponent : public ComponentBase
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override {};
	void Update(float dt) {};
	static const char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	//Checks if entity has stat
	bool HasStat(Stats::StatName stat)
	{
		auto it = m_stats.find(stat);
		if (it == m_stats.end())
			return false;
		return true;
	}

	//Returns value of stat
	double GetStat(Stats::StatName stat)
	{
		auto it = m_stats.find(stat);
		if (it == m_stats.end())
			return 0;
		return it->second;
	}

	//Sets value of stat. Returns true if stat exists otherwise returns false
	bool SetStat(Stats::StatName stat, double value)
	{
		auto it = m_stats.find(stat);
		if (it == m_stats.end())
			return false;
		it->second += value;
		return true;
	}

private:
	std::map<Stats::StatName, double> m_stats;
};

