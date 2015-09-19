#ifndef ATTACK_MANAGER_H
#define ATTACK_MANAGER_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "custom_defininitions.h"

/*
This system is a temporary method of attacking until I can find a cleaner way.
Basically every entity has an attack componenet. This component chnages when weapons and stats are updated.
On an attack command, this system creates an entity.
The creation of the component is done at the end of entity creation by passing the entity to the result of the CreateAttackComponent funciton.
*/

class AttackComponent;
class AttackManager
{
public:
	static StrongComponentPtr CreateAttackComponent();
	void Create
private:
	std::vector<std::shared_ptr<AttackComponent>> m_attackComponents;

};

#endif