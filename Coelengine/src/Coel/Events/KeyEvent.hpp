#pragma once
#include "Event.hpp"

namespace Coel
{
class KeyEvent : public Event
{
protected:
	int m_keyCode;
	KeyEvent(int keyCode)
		: m_keyCode(keyCode) {}

public:
	inline int getKeyCode() const { return m_keyCode; }
	EVENT_CLASS_CATEGORY(CategoryKeyboard | CategoryInput)
};

class KeyPressedEvent : public KeyEvent
{
	int m_repeatCount;

public:
	KeyPressedEvent(int keyCode, int repeatCount)
		: KeyEvent(keyCode), m_repeatCount(repeatCount) {}
	inline int getRepeatCount() const { return m_repeatCount; }
	EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode)
		: KeyEvent(keyCode) {}
	EVENT_CLASS_TYPE(KeyPressed)
};
} // namespace Coel