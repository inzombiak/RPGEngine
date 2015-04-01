#ifndef CUSTOM_DEFENITIONS_H
#define CUSTOM_DEFENITIONS_H

#include <memory>
#include <string>
#include <map>
using std::string;
namespace Equipment
{
	enum SlotName
	{
		RightHand = 0,
		LeftHand = 1,
		Armor = 2,
	};
	static std::map<string, SlotName> slotMap =
	{
		{ "RightHand", SlotName::RightHand },
		{ "LeftHand", SlotName::LeftHand },
		{ "Armor", SlotName::Armor },
	};
}


namespace Stats
{
	enum StatName
	{
		HP = 0,
		Damage = 1,
		Armor = 2,
		MovementSpeed = 3,
	};
	static std::map<string, StatName> statMap = 
	{ 
		{ "HP", StatName::HP },
		{ "Damage", StatName::Damage },
		{ "Armor", StatName::Armor },
		{ "Movement Speed", StatName::MovementSpeed }
	};
};

class Entity;
typedef std::shared_ptr<Entity> StrongEntityPtr;
typedef std::weak_ptr<Entity> WeakEntityPtr;

class ComponentBase;
typedef std::shared_ptr<ComponentBase> StrongComponentPtr;
typedef std::weak_ptr<ComponentBase> WeakComponentPtr;

class ItemComponent;
typedef std::shared_ptr<ItemComponent> StrongItemComponentPtr;
typedef std::weak_ptr<ItemComponent> WeakItemComponentPtr;

typedef unsigned int ComponentID;
typedef unsigned int ItemComponentID;
typedef unsigned int EntityNameID;
typedef unsigned int ItemID;

template<class Base, class Derived>
bool CheckConvertAndCastPtr(std::weak_ptr<Base> weakPtr, std::shared_ptr<Derived>& result)//ConvertToStrongPtr and CastComponentToDerived in one function!
{
	if (weakPtr.expired())
		return false;
	result = CastComponentToDerived<std::shared_ptr<Base>, Derived>(ConvertToStrongPtr(weakPtr));
	if (!result)
		return false;
	return true;
}
template<class Ptr, class SubClass>
Ptr GenericDerivedCreationFunction(void)
{
	return Ptr(new SubClass);
}

template <class T>
std::shared_ptr<T> ConvertToStrongPtr(std::weak_ptr<T> weakPtr)
{
	if (weakPtr.expired())
		return std::shared_ptr<T>();
	else
		return std::shared_ptr<T>(weakPtr);
}

template<class BasePointer, class DerivedClass>
std::shared_ptr<DerivedClass> CastComponentToDerived(BasePointer component)
{
	if (!component)
		return std::shared_ptr<DerivedClass>();
	else
		return std::static_pointer_cast<DerivedClass>(component);
}

template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return new SubType; }


typedef std::string EntityType;

//Taken from GCC4 - https://code.google.com/p/gamecode4/source/browse/trunk/Source/GCC4/Utilities/String.h
class HashedString
{
public:
	explicit HashedString(char const * const pIdentString)
		: m_ident(hash_name(pIdentString))
		, m_identStr(pIdentString)
	{
	}

	unsigned long getHashValue(void) const
	{

		return reinterpret_cast<unsigned long>(m_ident);
	}

	const std::string & getStr() const
	{
		return m_identStr;
	}

	static	void * hash_name(char const *  pIdentStr)
	{
		// Relatively simple hash of arbitrary text string into a
		// 32-bit identifier Output value is
		// input-valid-deterministic, but no guarantees are made
		// about the uniqueness of the output per-input
		//
		// Input value is treated as lower-case to cut down on false
		// separations cause by human mistypes. Sure, it could be
		// construed as a programming error to mix up your cases, and
		// it cuts down on permutations, but in Real World Usage
		// making this text case-sensitive will likely just lead to
		// Pain and Suffering.
		//
		// This code lossely based upon the adler32 checksum by Mark
		// Adler and published as part of the zlib compression
		// library sources.

		// largest prime smaller than 65536
		unsigned long BASE = 65521L;

		// NMAX is the largest n such that 255n(n+1)/2 +
		// (n+1)(BASE-1) <= 2^32-1
		unsigned long NMAX = 5552;

#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

		if (pIdentStr == NULL)
			return NULL;

		unsigned long s1 = 0;
		unsigned long s2 = 0;

		for (size_t len = strlen(pIdentStr); len > 0;)
		{
			unsigned long k = len < NMAX ? len : NMAX;

			len -= k;

			while (k >= 16)
			{
				DO16(pIdentStr);
				pIdentStr += 16;
				k -= 16;
			}

			if (k != 0) do
			{
				s1 += tolower(*pIdentStr++);
				s2 += s1;
			} while (--k);

			s1 %= BASE;
			s2 %= BASE;
		}

	#pragma warning(push)
	#pragma warning(disable : 4312)

		return reinterpret_cast<void *>((s2 << 16) | s1);

	#pragma warning(pop)
	#undef DO1
	#undef DO2
	#undef DO4
	#undef DO8
	#undef DO16
	};

	bool operator< (HashedString const & o) const
	{
		bool r = (getHashValue() < o.getHashValue());
		return r;
	}

	bool operator== (HashedString const & o) const
	{
		bool r = (getHashValue() == o.getHashValue());
		return r;
	}

private:

	// note: m_ident is stored as a void* not an int, so that in
	// the debugger it will show up as hex-values instead of
	// integer values. This is a bit more representative of what
	// we're doing here and makes it easy to allow external code
	// to assign event types as desired.

	void *             m_ident;
	std::string                m_identStr;
};


#endif
