#ifndef CUSTOM_DEFENITIONS_H
#define CUSTOM_DEFENITIONS_H

#include <memory>
#include <string>
#include <map>
#include "Component.h"
#include "SFGUI\sfgui.hpp"

using std::string;
namespace Slots
{
	enum SlotName
	{
		RightHand = 0,
		LeftHand = 1,
		Armor = 2,
		NaN = 3,
	};
	static std::map<string, SlotName> slotStringtoEnumMap =
	{
		{ "Right Hand", SlotName::RightHand },
		{ "Left Hand", SlotName::LeftHand },
		{ "Armor", SlotName::Armor },
		{ "NaN", SlotName::NaN }
	};
	static std::map<SlotName, string> slotEnumtoStringMap =
	{
		{ SlotName::RightHand, "Right Hand" },
		{ SlotName::LeftHand, "Left Hand" },
		{ SlotName::Armor, "Armor" },
		{ SlotName::NaN, "NaN" }
	};
};
namespace Stats
{

	enum StatName
	{
		HP = 0,
		Damage = 1,
		Armor = 2,
		MovementSpeed = 3,
	};
	static std::map<string, StatName> statStringtoEnumMap =
	{
		{ "HP", StatName::HP },
		{ "Damage", StatName::Damage },
		{ "Armor", StatName::Armor },
		{ "Movement Speed", StatName::MovementSpeed }
	};
	static std::map<StatName, string> statEnumtoStringMap =
	{
		{ StatName::HP, "HP" },
		{ StatName::Damage, "Damage" },
		{ StatName::Armor, "Armor" },
		{ StatName::MovementSpeed, "Movement Speed" }
	};
};

namespace UtilityFormFunctions
{
	/*
	Creates a widget based on valueType, value. Optional vector used for widgets that are made up of a dynamic list(entities/items)
	*/
	inline sfg::Widget::Ptr AddComponentWidgetToBox(string& valueType, string& value, sfg::Box::Ptr fieldBox, std::vector<string>& valueList = std::vector<string>())
	{
		//TODO: Use a template or something instead of if
		if (valueType.compare("stat") == 0)
		{
			sfg::ComboBox::Ptr statBox = sfg::ComboBox::Create();

			for (auto it = Stats::statEnumtoStringMap.begin(); it != Stats::statEnumtoStringMap.end(); ++it)
			{
				statBox->AppendItem(it->second);
			}
			statBox->SelectItem(Stats::statStringtoEnumMap[value]);
			fieldBox->Pack(statBox);
			return statBox;
		}
		else if (valueType.compare("slot") == 0)
		{
			sfg::ComboBox::Ptr slotBox = sfg::ComboBox::Create();

			for (auto it = Slots::slotEnumtoStringMap.begin(); it != Slots::slotEnumtoStringMap.end(); ++it)
			{
				slotBox->AppendItem(it->second);
			}
			slotBox->SelectItem(Slots::slotStringtoEnumMap[value]);
			fieldBox->Pack(slotBox);
			return slotBox;
		}
		else if (valueType.compare("item") == 0)
		{
			sfg::ComboBox::Ptr itemBox = sfg::ComboBox::Create();

			for (int i = 0; i < valueList.size(); ++i)
			{
				itemBox->AppendItem(valueList[i]);
				if (value.compare(valueList[i]) == 0)
					itemBox->SelectItem(i);
			}
			
			fieldBox->Pack(itemBox);
			return itemBox;
		}

		sfg::Entry::Ptr entry = sfg::Entry::Create();
		entry->SetText(value);
		fieldBox->Pack(entry);
		return entry;
	};
	inline string GetTextFromWidget(sfg::Widget::Ptr widget)
	{
		string widgetClass = widget->GetName();
		if (widgetClass.compare("ComboBox") == 0)
		{
			return std::dynamic_pointer_cast<sfg::ComboBox, sfg::Widget>(widget)->GetSelectedText();
		}
		else if (widgetClass.compare("Entry") == 0)
		{
			return std::dynamic_pointer_cast<sfg::Entry, sfg::Widget>(widget)->GetText();
		}
		else if (widgetClass.compare("Label") == 0)
		{
			return std::dynamic_pointer_cast<sfg::Label, sfg::Widget>(widget)->GetText();
		}
		return "";
	}
}

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
