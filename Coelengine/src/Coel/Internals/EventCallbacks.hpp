#pragma once

#include "Coel/Events.hpp"

namespace Coel {
	namespace Internals {
#define IMPLIMENT_EVENT_CALLBACK(name)               \
	static void empty##name(const name##Event &e) {} \
	static void (*on##name##Callback)(const name##Event &) = &empty##name;

		IMPLIMENT_EVENT_CALLBACK(KeyPressed)
		IMPLIMENT_EVENT_CALLBACK(KeyReleased)
		IMPLIMENT_EVENT_CALLBACK(KeyRepeated)

		IMPLIMENT_EVENT_CALLBACK(MouseMoved)
		IMPLIMENT_EVENT_CALLBACK(MousePressed)
		IMPLIMENT_EVENT_CALLBACK(MouseReleased)
		IMPLIMENT_EVENT_CALLBACK(MouseScrolled)

		IMPLIMENT_EVENT_CALLBACK(WindowMoved)
		IMPLIMENT_EVENT_CALLBACK(WindowResized)
		IMPLIMENT_EVENT_CALLBACK(WindowClosed)
		IMPLIMENT_EVENT_CALLBACK(WindowFocused)
		IMPLIMENT_EVENT_CALLBACK(WindowLostFocus)
	} // namespace Internals
} // namespace Coel