#ifndef EQUIPMENT_H
#define EQUIPMENT_H


class Equipment
{
public:
	
	void Use()
	{
		m_equipped = !m_equipped;
	};

private:
	bool m_equipped;
	int m_power;
};

#endif
