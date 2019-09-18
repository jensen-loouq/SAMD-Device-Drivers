#ifndef __MBD_EVENT_H__
#define __MBD_EVENT_H__

#include "Delegate.h"

namespace mbd
{
	template <typename T>
	class event;	
}



namespace mbd
{
	template <typename RETTy_, typename... ARGS>
	class event<delegate<RETTy_(ARGS...)>>
	{		
		using EventHandler = delegate<RETTy_(ARGS...)>;		
		using eventHandlerType = RETTy_(*)(ARGS...);

	public:
		constexpr event() : m_eventHandler([](ARGS...) {}) {}

	public:
		operator eventHandlerType() { return m_eventHandler; }
		void operator +=(const EventHandler evHandler) { m_eventHandler = evHandler; }

	private:
		EventHandler m_eventHandler;
	};
}

#endif // !__MBD_EVENT_H__

