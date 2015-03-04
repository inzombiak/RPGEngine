#ifndef INTERFACES_H
#define INTERFACES_H

<<<<<<< HEAD
#include <memory.h>

class ObjectComponent;

typedef shared_ptr<ObjectComponent> StrongObjectComponentPtr;
=======
#include <memory>

class EntityComponent;
class Entity;

typedef std::shared_ptr<Entity> StrongEntityPtr;
typedef std::shared_ptr<EntityComponent> StrongEntityComponentPtr;


typedef unsigned int ComponentID;
typedef unsigned int EntityID;
>>>>>>> 3a4d79cf25455d100c03d851ae957c28a2dfb440

#endif