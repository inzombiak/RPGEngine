#include "ComponentBase.h"
#include "custom_defininitions.h"
#include <string>

//Used for defining health, mana, stamina, breath etc.
class VitalsComponent : public ComponentBase
{
public:
	enum VitalType
	{
		Health = 0,
		Mana = 1,
		Stamina = 2,
		NaN = 3
	};
	struct Vital
	{
		VitalType type;
		int current;
		int max;
	};
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override{};
	virtual void Update(float dt) {};
	static const char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}
	bool GetVital(VitalType type, Vital& vital)
	{
		std::map<VitalType, Vital>::iterator it = m_vitals.find(type);
		if (it == m_vitals.end())
			return false;
		else
			vital = it->second;
		return true;
	}
	VitalType StringtoVitalType(std::string type)
	{
		if (type.compare("Health") == 0)
			return VitalType::Health;
		else if (type.compare("Stamina") == 0)
			return VitalType::Stamina;
		else if (type.compare("Mana") == 0)
			return VitalType::Mana;
		else
			return VitalType::NaN;
	}
	bool SetVitalMax(VitalType type, int newMax)
	{	
		std::map<VitalType, Vital>::iterator it = m_vitals.find(type);
		if (it == m_vitals.end())
			return false;
		it->second.max = newMax;
		return true;
	}
	bool SetVitalCurrent(VitalType type, int newCurr)
	{
		std::map<VitalType, Vital>::iterator it = m_vitals.find(type);
		if (it == m_vitals.end())
			return false;
		if (newCurr > it->second.max)
			newCurr = it->second.max;
		it->second.current = newCurr;
		return true;
	}
private:
	std::map<VitalType,Vital> m_vitals;
};