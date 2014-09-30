#ifndef INTERFACES_H
#define INTERFACES_H

#include <memory>

class EntityComponent;
class Entity;

typedef std::shared_ptr<Entity> StrongEntityPtr;
typedef std::shared_ptr<EntityComponent> StrongEntityComponentPtr;


typedef unsigned int ComponentID;
typedef unsigned int EntityID;

#endif