#pragma once

#include "Coel/Events.hpp"

namespace Coel {
	namespace Internals {
#define __IMPLIMENT_EVENT_CALLBACK(_namespace_, _eventName_)                                         \
	static void empty##_namespace_##_eventName_##Func(const _namespace_::##_eventName_##Event &e) {} \
	static void (*on##_namespace_##_eventName_##Callback)(const _namespace_::_eventName_##Event &) = &empty##_namespace_##_eventName_##Func;

		__IMPLIMENT_EVENT_CALLBACK(Key, Press)
		__IMPLIMENT_EVENT_CALLBACK(Key, Release)
		__IMPLIMENT_EVENT_CALLBACK(Key, Repeat)

		__IMPLIMENT_EVENT_CALLBACK(Mouse, Move)
		__IMPLIMENT_EVENT_CALLBACK(Mouse, Press)
		__IMPLIMENT_EVENT_CALLBACK(Mouse, Release)
		__IMPLIMENT_EVENT_CALLBACK(Mouse, Scroll)

		__IMPLIMENT_EVENT_CALLBACK(Window, Move)
		__IMPLIMENT_EVENT_CALLBACK(Window, Resize)
		__IMPLIMENT_EVENT_CALLBACK(Window, Close)
		__IMPLIMENT_EVENT_CALLBACK(Window, Focus)
		__IMPLIMENT_EVENT_CALLBACK(Window, Defocus)
	} // namespace Internals
} // namespace Coel