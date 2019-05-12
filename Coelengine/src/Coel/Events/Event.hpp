#pragma once
#include "clpch.hpp"
#include "Coel/Core.hpp"

namespace Coel
{
enum class EventType
{
	None = 0,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,
	AppTick,
	AppUpdate,
	AppRender,
	KeyPressed,
	KeyReleased,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled
};

enum EventCategory
{
	None = 0,
	CategoryApplication = 1 << 0,
	CategoryInput = 1 << 1,
	CategoryKeyboard = 1 << 2,
	CategoryMouse = 1 << 3,
	CategoryMouseButton = 1 << 4
};

class Event
{
public:
	static Event *s_current;

	bool handled = false;
	virtual EventType getEventType() const = 0;
	virtual int getCategoryFlags() const = 0;

	template <typename T>
	static bool dispatch(std::function<bool(T &)> func)
	{
		if (s_current->getEventType() == T::getStaticType())
		{
			s_current->handled = func(*(T *)s_current);
			return true;
		}
		return false;
	}
};
} // namespace Coel

#define EVENT_CLASS_CATEGORY(category) \
	virtual int getCategoryFlags() const override { return category; }
#define EVENT_CLASS_TYPE(type)                                   \
	static EventType getStaticType() { return EventType::type; } \
	EventType getEventType() const override { return getStaticType(); }
