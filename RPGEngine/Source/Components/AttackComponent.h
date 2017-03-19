#pragma once

#include "IComponent.h"

class AttackComponent : public IComponent
{
public:
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override {};
	void Update(float dt) {};

	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	static const char* COMPONENT_NAME;

	void InitWithValues(double damage, double power, double weight)
	{
		m_damage = damage;
		m_power = power;
		m_weight = weight;
	}

	void SetDamage(double newDamage)
	{
		m_damage = newDamage;
	}
	double GetDamage() const
	{
		return m_damage;
	}

	void SetPower(double newPower)
	{
		m_power = newPower;
	}
	double GetPower() const
	{
		return m_power;
	}

	void SetWeight(double newWeight)
	{
		m_weight = newWeight;
	}
	double GetWeight() const
	{
		return m_weight;
	}

private:
	//Damage of the attack, negative means healing
	double m_damage;
	//Power of the attack, affects the path of movement
	double m_power;
	//Weight of the weapon, affects the path of movement
	double m_weight;
	//Filepath to the image of the attack
	std::string m_textureFilepath;

	//AttackPatch m_path
	//LuaScriptSpecial m_special
	//vector<textures> m_image
};

