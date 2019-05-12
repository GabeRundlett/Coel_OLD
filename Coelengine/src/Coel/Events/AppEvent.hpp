#pragma once
#include "Event.hpp"

namespace Coel
{
class AppUdateEvent : public Event
{
public:
	AppUdateEvent() {}
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(AppUpdate)
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() {}
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(AppTick)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(AppRender)
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(WindowClose)
};

class WindowResizeEvent : public Event
{
	unsigned int m_width, m_height;

public:
	WindowResizeEvent(unsigned int w, unsigned int h)
		: m_width(w), m_height(h) {}
	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(WindowResize)
};
} // namespace Coel