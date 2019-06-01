#pragma once

#include "Events.hpp"

namespace Engine {
	namespace WindowManager {
		unsigned int init();
		void update();
		void close();
		int shouldRun();
	} // namespace WindowManager
#define __ENGINE_DECLARE_EVENT_CALLBACK_SETTER(name) \
	void setOn##name##Callback(void (*func)(const name##Event &))

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(KeyPressed);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(KeyReleased);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(KeyRepeated);

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(MouseMoved);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(MousePressed);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(MouseReleased);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(MouseScrolled);

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(WindowMoved);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(WindowResized);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(WindowClosed);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(WindowFocused);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(WindowLostFocus);
} // namespace Engine
