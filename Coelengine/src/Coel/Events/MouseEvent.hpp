#pragma once
#include "Event.hpp"

namespace Coel
{
class MouseMovedEvent : public Event
{
	float m_diffX, m_diffY;

public:
	MouseMovedEvent(float x, float y)
		: m_diffX(x), m_diffY(y) {}
	inline float getX() const { return m_diffX; }
	inline float getY() const { return m_diffY; }
	EVENT_CLASS_CATEGORY(CategoryMouse | CategoryInput)
	EVENT_CLASS_TYPE(MouseMoved)
};

class MouseScrolledEvent : public Event
{
	float m_diffX, m_diffY;

public:
	MouseScrolledEvent(float x, float y)
		: m_diffX(x), m_diffY(y) {}
	inline float getX() const { return m_diffX; }
	inline float getY() const { return m_diffY; }
	EVENT_CLASS_CATEGORY(CategoryMouse | CategoryInput)
	EVENT_CLASS_TYPE(MouseScrolled)
};

class MouseButtonEvent : public Event
{
protected:
	int m_button;
	MouseButtonEvent(int button)
		: m_button(button) {}

public:
	inline int getMouseButton() const { return m_button; }
	EVENT_CLASS_CATEGORY(CategoryMouse | CategoryMouseButton | CategoryInput)
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button)
		: MouseButtonEvent(button) {}
	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button)
		: MouseButtonEvent(button) {}
	EVENT_CLASS_TYPE(MouseButtonReleased)
};
} // namespace Coel
