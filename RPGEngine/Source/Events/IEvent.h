#pragma once
#include "IEventData.h"
class IEvent
{
public:
	IEvent();
	virtual ~IEvent();

	virtual void SetData(IEventData* data) = 0;
	virtual IEventData* GetData() = 0;
}; 

