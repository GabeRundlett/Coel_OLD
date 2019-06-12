#pragma once

#include "Events.hpp"

namespace Coel {
	namespace Application {
		constexpr static const unsigned int TICK_RATE = 128;
		void start();
		void reset();
		void close();
		void maximize();
		void minimize();
	} // namespace Application
	void setOnStartCallback(void (*func)());
	void setOnResetCallback(void (*func)());
	void setOnCloseCallback(void (*func)());
	void setOnUpdateCallback(void (*func)());
	void setOnTickCallback(void (*func)());
} // namespace Coel
