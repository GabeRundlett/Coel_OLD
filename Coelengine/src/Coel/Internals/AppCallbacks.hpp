#pragma once

#include "Coel/Events.hpp"

namespace Coel {
	namespace Internals {
		static void emptyFunction() {}

		static void (*onStartCallback)() = &emptyFunction;
		static void (*onResetCallback)() = &emptyFunction;
		static void (*onCloseCallback)() = &emptyFunction;
		static void (*onUpdateCallback)() = &emptyFunction;
		static void (*onTickCallback)() = &emptyFunction;
	} // namespace Internals
} // namespace Coel