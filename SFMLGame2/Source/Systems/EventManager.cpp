#include "EventManager.h"

bool EventManager::AddEventListener(EventDefs::EventType type, EventDelegate callBack)
{
	auto callbackIt = m_eventCallbacks.find(type);
	if (callbackIt == m_eventCallbacks.end())
		m_eventCallbacks[type] = EventDelegateList();

	auto& callbackList = m_eventCallbacks[type];
	for (auto it = callbackList.begin(); it != callbackList.end(); ++it)
	{
		if (it->target<EventDelegate>() == callBack.target<EventDelegate>())
			return false;
	}

	callbackList.push_back(callBack);

	return true;
}

bool EventManager::RemoveEventListener(EventDefs::EventType type, EventDelegate callBack)
{
	auto callbackIt = m_eventCallbacks.find(type);
	if (callbackIt == m_eventCallbacks.end())
		return false;

	auto& callbackList = callbackIt->second;
	for (auto it = callbackList.begin(); it != callbackList.end(); ++it)
	{
		if (it->target<EventDelegate>() == callBack.target<EventDelegate>())
		{
			callbackList.erase(it);
			return true;
		}
	}

	return false;
}

void EventManager::QueueEvent(EventDefs::EventType type, IEventData* data, bool isSynchronous)
{
	if (!m_isSyncEventList.at(type) && !isSynchronous)
	{
		m_eventQueue.push(data);

		return;
	}
		
	auto callBackListIt = m_eventCallbacks.find(type);
	if (callBackListIt == m_eventCallbacks.end())
	{
		delete data;
		return;
	}

	auto& callBackList = callBackListIt->second;

	for (auto it = callBackList.begin(); it != callBackList.end(); ++it)
		(*it)(data);

	data->SetDelete(true);
	m_eventQueue.push(data);
}

void EventManager::ProcessEvents()
{
	while (!m_eventQueue.empty())
	{
		IEventData* eData = m_eventQueue.front();
		EventDefs::EventType eType = eData->GetEventType();

		auto callBackListIt = m_eventCallbacks.find(eType);
		if (callBackListIt == m_eventCallbacks.end() || eData->ShouldDelete())
		{
				delete eData;
				m_eventQueue.pop();
				continue;
		}

		auto& callBackList = callBackListIt->second;

		for (auto it = callBackList.begin(); it != callBackList.end(); ++it)
			(*it)(eData);

		delete eData;
		m_eventQueue.pop();
	}
}

EventManager::SynchronousEventMap EventManager::SetSynchronousEvents()
{
	EventManager::SynchronousEventMap result;

	result[EventDefs::SELECT_OBJECT_CLICK] = true;
	result[EventDefs::GET_PHYSICS_DATA] = true;
	result[EventDefs::GET_OBJECT_SETTINGS] = true;

	result[EventDefs::NULL_TYPE] = false;
	result[EventDefs::CREATE_OBJECT] = false;
	result[EventDefs::CREATE_PHYSICS_COMPONENT] = false;
	result[EventDefs::CREATE_RENDER_COMPONENT] = false;
	result[EventDefs::CREATE_CONSTRAINT] = false;
	result[EventDefs::DELETE_OBJECT] = false;
	result[EventDefs::DELETE_PHYSICS_COMPONENT] = false;
	result[EventDefs::DELETE_RENDER_COMPONENT] = false;
	result[EventDefs::APPLY_FORCE] = false;

	return result;
}

EventManager* EventManager::m_instance = 0;
EventManager::EventQueue EventManager::m_eventQueue;
EventManager::CallbackMap EventManager::m_eventCallbacks;
const EventManager::SynchronousEventMap EventManager::m_isSyncEventList = EventManager::SetSynchronousEvents();