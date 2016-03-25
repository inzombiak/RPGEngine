#pragma once
#include "..\Events\IEventData.h"

#include <functional>
#include <list>
#include <map>
#include <queue>

typedef std::function<void(IEventData*)> EventDelegate;

class EventManager
{
public:
	
	bool AddEventListener(EventDefs::EventType type, EventDelegate callBack);
	bool RemoveEventListener(EventDefs::EventType type, EventDelegate callBack);
	void QueueEvent(EventDefs::EventType type, IEventData* data, bool isSynchronous);
	void ProcessEvents();

	static EventManager* GetInstance()
	{
		if (!m_instance)
			m_instance = new EventManager();

		return m_instance;
	}

	static void DestroyInstance()
	{
		if (m_instance)
			delete m_instance;

		m_instance = 0;
	}

private:
	typedef std::list<EventDelegate> EventDelegateList;
	typedef std::map<EventDefs::EventType, EventDelegateList> CallbackMap;
	typedef std::queue<IEventData*> EventQueue;
	typedef std::map<EventDefs::EventType, bool> SynchronousEventMap;

	static SynchronousEventMap SetSynchronousEvents();

	EventManager() {};
	
	static EventManager* m_instance;
	static CallbackMap m_eventCallbacks;
	static EventQueue m_eventQueue;
	static const SynchronousEventMap m_isSyncEventList;
};

