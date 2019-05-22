#pragma once
#include "Event.hpp"

namespace Coel
{
class AppUpdateEvent : public Event
{
	float m_time;
public:
	AppUpdateEvent(float elapsed)
		: m_time(elapsed) {}
	inline float getTime() const { return m_time; }
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(AppUpdate)
};

class AppTickEvent : public Event
{
	float m_time;
public:
	AppTickEvent(float elapsed)
		: m_time(elapsed) {}
	inline float getTime() const { return m_time; }
	EVENT_CLASS_CATEGORY(CategoryApplication)
	EVENT_CLASS_TYPE(AppTick)
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