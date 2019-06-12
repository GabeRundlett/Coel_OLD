#pragma once

#include "Events.hpp"

namespace Coel {
	namespace WindowManager {
		unsigned int init();
		void update();
		int shouldRun();
		void shutdown();
		void maximize();
		void minimize();
	} // namespace WindowManager
#define __ENGINE_DECLARE_EVENT_CALLBACK_SETTER(_namespace_, _eventName_) \
	void setOn##_namespace_##_eventName_##Callback(void (*func)(const Event::_namespace_::_eventName_ &))

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Key, Press);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Key, Release);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Key, Repeat);

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Mouse, Move);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Mouse, Press);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Mouse, Release);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Mouse, Scroll);

	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Window, Move);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Window, Resize);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Window, Close);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Window, Focus);
	__ENGINE_DECLARE_EVENT_CALLBACK_SETTER(Window, Defocus);
} // namespace Coel
